#include <Secrets.h>
#include <Arduino.h>
#include <Commands.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <Twang.h>
#include <ArduinoOTA.h>

#define SERIAL_HEADER 42
#define SERIAL_READY_TO_SEND 0xAA
#define SERIAL_CLEAR_TO_SEND 0xBB
#define SERIAL_REQUEST_RESYNC 0xCC

const uint16_t SERIAL_BUFFER_SIZE = 1024;
char serial_buffer[SERIAL_BUFFER_SIZE];
const uint8_t SERIAL_PACKET_SIZE = 2 + COMMAND_DATA_SIZE;
const uint8_t SERIAL_TIMEOUT = 100;

#ifdef LED_STRUCTURE_TROMMEL
#include <Trommel.h>
Trommel leds = Trommel();
#endif

// udp settings
#define UDP_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char udp_buffer[UDP_BUFFER_SIZE];
WiFiUDP udp;
bool udp_up = false;
bool ota_up = false;
bool wasdisconnected = true;

Commands commands = Commands();

Command stashed_command[1];//for restoring command zero after reconnect
bool commandstashed = false;

// PINS
#ifdef UART_COMMANDS_VIA_PINS
HardwareSerial CommandSerial(2);
#endif
#ifdef UART_COMMANDS_VIA_USB
HardwareSerial CommandSerial(0);
#endif

void flush_serial()
{
  while (CommandSerial.available())
  {
    int byte = CommandSerial.read();
    Serial.print("Flushing from serial: ");
    Serial.println(byte);
  }
}

void setup() {
  Serial.begin(921600);
  CommandSerial.begin(921600);
  while (!Serial || !CommandSerial)
  {
    // do nothing
  }
  
  //this part is for over-the-air (OTA) updates
  //if you mess this up, you'll have to climb the ladder of shame ;)
  Serial.println("Setting up OTA");
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("trommel");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  ArduinoOTA.setPasswordHash(ota_password_hash);
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  //End of OTA section

  CommandSerial.setTimeout(SERIAL_TIMEOUT);

  Serial.println("\n\tLED Controller\r\n");

  Serial.println("Starting up...");

  commands.init(&leds);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Initiating LED structure: ");
  leds.write_info();

  Serial.println("Sending resync request");

  CommandSerial.write(SERIAL_REQUEST_RESYNC);
  // artnet.begin(artnet_ip);
  // artnet.setArtDmxCallback(led_structure.artnet_packet_callback);
  // artnet.setArtSyncCallback(commands.artnet_sync_callback);

  Serial.println("Startup complete, going into render loop");
}

void wifi_init() {
  WiFi.disconnect(true);
  Serial.printf("Connecting to ssid %s\n", ssid);
  WiFi.begin(ssid, password);
}

void upd_init() {
  Serial.printf("Initializing UDP...\n");
  udp.begin(LISTEN_PORT);
  Serial.printf("Done. Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
  udp_up = true;
}

void print_wifi_status (int wifi_status) {
  if (wifi_status == WL_CONNECTED) {
    Serial.printf("WiFi status: WL_CONNECTED\n");
  } else if (wifi_status == WL_NO_SHIELD) {
    Serial.printf("WiFi status: WL_NO_SHIELD\n");
  } else if (wifi_status == WL_IDLE_STATUS) {
    Serial.printf("WiFi status: WL_IDLE_STATUS\n");
  } else if (wifi_status == WL_NO_SSID_AVAIL) {
    Serial.printf("WiFi status: WL_NO_SSID_AVAIL\n");
  } else if (wifi_status == WL_SCAN_COMPLETED) {
    Serial.printf("WiFi status: WL_SCAN_COMPLETED\n");
  } else if (wifi_status == WL_CONNECT_FAILED) {
    Serial.printf("WiFi status: WL_CONNECT_FAILED\n");
  } else if (wifi_status == WL_CONNECTION_LOST) {
    Serial.printf("WiFi status: WL_CONNECTION_LOST\n");
  } else if (wifi_status == WL_DISCONNECTED) {
    Serial.printf("WiFi status: WL_DISCONNECTED\n");
  } else {
    Serial.printf("WiFi status: %d\n", wifi_status);
  }
}

void stash_command_0(){//copy command zero to stash
  if (commandstashed) return;
  stashed_command[0].type = command_buffer[0].type;
  for (uint8_t i =0; i < COMMAND_DATA_SIZE;i++)
    stashed_command[0].data[i]=command_buffer[0].data[i];
  commandstashed = true;
}

void restore_command_0(){//copy stash into command zero
  if (!commandstashed) return;
  command_buffer[0].type = stashed_command[0].type;
  for (uint8_t i =0; i < COMMAND_DATA_SIZE;i++)
    command_buffer[0].data[i]=stashed_command[0].data[i];
  commandstashed = false;
}

void loop()
{

  int wifi_status = WiFi.status();

  if(wifi_status == WL_CONNECTED) {
    if (wasdisconnected){//if this is a reconnect restore the previous effect
      restore_command_0();
      commands.run(); 
      wasdisconnected = false;
    }
    //this part is for over-the-air (OTA) updates
    //if you mess this up, you'll have to climb the ladder of shame ;)
    if (!ota_up) {
        ArduinoOTA.begin();
        ota_up = true;
    }
    ArduinoOTA.handle();
    //end of OTA section
    
    if (udp_up) {
      // wifi and upd is up, check if we received a UDP packet
      int packet_size = udp.parsePacket();
      if (packet_size) {
        Serial.printf("Received command with size %d from %s, port %d\n", packet_size, udp.remoteIP().toString().c_str(), udp.remotePort());
        int len = udp.read(udp_buffer, UDP_BUFFER_SIZE);
        if (len > 0) {
          udp_buffer[len] = 0;
        }
        // Serial.printf("UDP packet contents: %s\n", udp_buffer);
        commands.process((char *) udp_buffer);
      }
      commands.run();
    } else {
      print_wifi_status(wifi_status);
      upd_init();
    }

  } else if(wifi_status == WL_IDLE_STATUS || wifi_status == WL_DISCONNECTED) {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    stash_command_0();//save command zero for restoring when reconnecting
    char command_buffer[] =  {0x00,COMMAND_SINGLE_COLOR,STRIP_INDEX_ALL,HUE_GREEN,255};
    commands.process((char *) command_buffer);
    commands.run();
    delay(500);
  } else {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    stash_command_0();//save command zero for restoring when reconnecting
    char command_buffer[] =  {0x00,COMMAND_SINGLE_COLOR,STRIP_INDEX_ALL,HUE_ORANGE,255};
    commands.process((char *) command_buffer);
    commands.run();
    wifi_init();
  }
}

