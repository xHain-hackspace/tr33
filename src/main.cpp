#include <Secrets.h>
#include <Arduino.h>
#include <Commands.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <Twang.h>
#include <ArduinoOTA.h>

#ifdef LED_STRUCTURE_TR33
#include <Tr33.h>
Tr33 leds = Tr33();
#endif
#ifdef LED_STRUCTURE_DODE
Dode leds = Dode();
#endif
#ifdef LED_STRUCTURE_KELLER
#include <Keller.h>
Keller leds = Keller();
#endif
#ifdef LED_STRUCTURE_WAND
#include <Wand.h>
Wand leds = Wand();
#endif
#ifdef LED_STRUCTURE_TROMMEL
#include <Trommel.h>
Trommel leds = Trommel();
#endif
#ifdef LED_STRUCTURE_WOLKE
#include <Wolke.h>
Wolke leds = Wolke();
#endif

Commands commands = Commands();

// ----- UART -------------------------------

#define SERIAL_HEADER 42
#define SERIAL_READY_TO_SEND 0xAA
#define SERIAL_CLEAR_TO_SEND 0xBB
#define SERIAL_REQUEST_RESYNC 0xCC

const uint16_t SERIAL_BUFFER_SIZE = 1024;
char serial_buffer[SERIAL_BUFFER_SIZE];
const uint8_t SERIAL_PACKET_SIZE = 2 + COMMAND_DATA_SIZE;
const uint8_t SERIAL_TIMEOUT = 100;

#ifdef COMMANDS_VIA_UART_PINS
HardwareSerial CommandSerial(2);
#endif
#ifdef COMMANDS_VIA_UART_USB
HardwareSerial CommandSerial(0);
#endif
#ifdef COMMANDS_VIA_WIFI
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

void uart_loop()
{
  int byte;
  int commandCount;
  int bytes_read;

  byte = CommandSerial.read();

  if (byte == SERIAL_READY_TO_SEND)

  {
    CommandSerial.write(SERIAL_CLEAR_TO_SEND);
    long cts_send_time = millis();

    while (byte != SERIAL_HEADER && millis() < cts_send_time + SERIAL_TIMEOUT)
    {
      byte = CommandSerial.read();
    }

    if (byte == SERIAL_HEADER)
    {
      commandCount = CommandSerial.read();

      for (int i = 0; i < commandCount; i++)
      {
        bytes_read = CommandSerial.readBytes(serial_buffer, SERIAL_PACKET_SIZE);
        if (bytes_read == SERIAL_PACKET_SIZE)
        {
          commands.process((char *)serial_buffer);
        }
        else
        {
          Serial.print("Incomplete message, bytes read: ");
          Serial.println(bytes_read);
        }
      }
    }
    else
    {
      // Serial.print("Expected header, got: ");
      // Serial.println(byte);
    }
  }
  else if (byte != -1)
  {
    Serial.print("Expected RTS, got: ");
    Serial.println(byte);
    Serial.println("Flushing serial");
    flush_serial();
    // CommandSerial.write(SERIAL_REQUEST_RESYNC);
  }

  commands.run();
}

void uart_setup()
{
  CommandSerial.begin(921600);
  while (!CommandSerial)
  {
    // do nothing
  }
  CommandSerial.setTimeout(SERIAL_TIMEOUT);
  Serial.println("UART: Sending resync request");

  CommandSerial.write(SERIAL_REQUEST_RESYNC);
}

// ----- WIFI -------------------------------

// udp settings
#define UDP_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char udp_buffer[UDP_BUFFER_SIZE];
WiFiUDP udp;
bool udp_up = false;
bool ota_up = false;
bool wasdisconnected = true;

Command stashed_command[1]; //for restoring command zero after reconnect
bool commandstashed = false;
#if defined(WIFI_HOSTNAME)
char hostname[] = WIFI_HOSTNAME;
#else
char hostname[] = "esp32";
#endif

const char *ota_password_hash = "d3d57181ad9b5b2e5e82a6c0b94ba22e";

void wifi_init()
{
  WiFi.disconnect(true);
  delay(10);                                          // for some reason this makes reconnects more reliable
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE); // needs to be here for the hostname to work...
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  Serial.printf("Connecting to ssid %s, setting hostname to %s\n", ssid, hostname);
  WiFi.begin(ssid, password);
}

void upd_init()
{
  Serial.printf("Initializing UDP...\n");
  udp.begin(LISTEN_PORT);
  Serial.printf("Done. Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
  udp_up = true;
}

void print_wifi_status(int wifi_status)
{
  if (wifi_status == WL_CONNECTED)
  {
    Serial.printf("WiFi status: WL_CONNECTED\n");
  }
  else if (wifi_status == WL_NO_SHIELD)
  {
    Serial.printf("WiFi status: WL_NO_SHIELD\n");
  }
  else if (wifi_status == WL_IDLE_STATUS)
  {
    Serial.printf("WiFi status: WL_IDLE_STATUS\n");
  }
  else if (wifi_status == WL_NO_SSID_AVAIL)
  {
    Serial.printf("WiFi status: WL_NO_SSID_AVAIL\n");
  }
  else if (wifi_status == WL_SCAN_COMPLETED)
  {
    Serial.printf("WiFi status: WL_SCAN_COMPLETED\n");
  }
  else if (wifi_status == WL_CONNECT_FAILED)
  {
    Serial.printf("WiFi status: WL_CONNECT_FAILED\n");
  }
  else if (wifi_status == WL_CONNECTION_LOST)
  {
    Serial.printf("WiFi status: WL_CONNECTION_LOST\n");
  }
  else if (wifi_status == WL_DISCONNECTED)
  {
    Serial.printf("WiFi status: WL_DISCONNECTED\n");
  }
  else
  {
    Serial.printf("WiFi status: %d\n", wifi_status);
  }
}

void wifi_setup()
{
  //this part is for over-the-air (OTA) updates
  //if you mess this up, you'll have to climb the ladder of shame ;)
  Serial.println("Setting up OTA");
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("trommel-test");
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
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });
  //End of OTA section
}

void wifi_loop()
{

  int wifi_status = WiFi.status();

  if (wifi_status == WL_CONNECTED)
  {
    if (wasdisconnected)
    { //if this is a reconnect restore the previous effect
      char command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_DISABLE};
      commands.process((char *)command_buffer);
      commands.run();
      wasdisconnected = false;
    }
    //this part is for over-the-air (OTA) updates
    //if you mess this up, you'll have to climb the ladder of shame ;)
    if (!ota_up)
    {
      ArduinoOTA.begin();
      ota_up = true;
    }
    ArduinoOTA.handle();
    //end of OTA section

    if (udp_up)
    {
      // wifi and upd is up, check if we received a UDP packet
      int packet_size = udp.parsePacket();
      if (packet_size)
      {
        // Serial.printf("Received command with size %d from %s, port %d\n", packet_size, udp.remoteIP().toString().c_str(), udp.remotePort());
        int len = udp.read(udp_buffer, UDP_BUFFER_SIZE);
        if (len > 0)
        {
          udp_buffer[len] = 0;
        }
        // Serial.printf("UDP packet contents: %s\n", udp_buffer);
        commands.process((char *)udp_buffer);
      }
      commands.run();
    }
    else
    {
      print_wifi_status(wifi_status);
      upd_init();
    }
  }
  else if (wifi_status == WL_IDLE_STATUS || wifi_status == WL_DISCONNECTED)
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    char command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_SINGLE_COLOR, STRIP_INDEX_ALL, HUE_GREEN, 255};
    commands.process((char *)command_buffer);
    commands.run();
    delay(500);
  }
  else
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    char command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_SINGLE_COLOR, STRIP_INDEX_ALL, HUE_ORANGE, 255};
    commands.process((char *)command_buffer);
    commands.run();
    wifi_init();
  }
}

// --- MAIN ----------------------------------

void setup()
{
  Serial.begin(921600);
  while (!Serial)
  {
    // do nothing
  }

  Serial.println("\n\tLED Controller\r\n");

  Serial.println("Starting up...");
  commands.init(&leds);

  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Initiating LED structure: ");
  leds.write_info();

#ifdef COMMANDS_VIA_WIFI
  Serial.println("Receiving Commands via WiFi. Running setup.");
  wifi_setup();
#endif
#ifdef COMMANDS_VIA_UART_PINS
  Serial.println("Receiving Commands via uart pins. Running setup.");
  uart_setup();
#endif
#ifdef COMMANDS_VIA_UART_USB
  Serial.println("Receiving Commands via uart usb. Running setup.");
  uart_setup();
#endif

  Serial.println("Startup complete, going into render loop");
}

void loop()
{
#ifdef COMMANDS_VIA_WIFI
  wifi_loop();
#endif
#ifdef COMMANDS_VIA_UART_PINS
  uart_loop();
#endif
#ifdef COMMANDS_VIA_UART_USB
  uart_loop();
#endif
}
