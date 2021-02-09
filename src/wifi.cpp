#include <Arduino.h>
#include <Commands.h>
// #include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#if defined(SECRETS_FILE)
#include SECRETS_FILE
#else
#include <Secrets.h>
#endif

// UDP settings
#define UDP_BUFFER_SIZE 1500
#define LISTEN_PORT 1337
uint8_t udp_buffer[UDP_BUFFER_SIZE];
WiFiUDP udp;
bool udp_up = false;
bool ota_up = false;
bool wasdisconnected = true;

#if defined(WIFI_HOSTNAME)
char hostname[] = WIFI_HOSTNAME;
#else
char hostname[] = "esp32_tr33";
#endif

const char *ota_password_hash = "d3d57181ad9b5b2e5e82a6c0b94ba22e";

// const char resync_host[] = "";
const int resync_port = 1337;
const int resync_reqest_length = 6;
const char resync_request_content[] = "resync";

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
  // send resync request
  Serial.printf("Sending resync request to %d.%d.%d.%d:%d\n", resync_host[0], resync_host[1], resync_host[2], resync_host[3], resync_port);
  udp.beginPacket(resync_host, resync_port);
  udp.write((uint8_t *)resync_request_content, resync_reqest_length);
  udp.endPacket();
  Serial.println("... done");

  udp_up = true;
}

void print_wifi_status(int wifi_status)
{
  if (wifi_status == WL_CONNECTED)
  {
    Serial.printf("WiFi status: WL_CONNECTED\n");
    Serial.printf("IP:  %s\n", WiFi.localIP().toString().c_str());
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

void wifi_loop(Commands commands)
{
  int wifi_status = WiFi.status();

  if (wifi_status == WL_CONNECTED)
  {
    if (wasdisconnected)
    { //if this is a reconnect restore the previous effect
      print_wifi_status(wifi_status);
      uint8_t command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_DISABLE};
      commands.process(command_buffer);
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

#ifdef COMMANDS_VIA_WIFI
    if (udp_up)
    {
      // wifi and upd is up, check if we received a UDP packet
      while (udp.parsePacket())
      {
        // Serial.println("debug");
        // Serial.printf("%d: Received command with size %d from %s, port %d\n", millis(), packet_size, udp.remoteIP().toString().c_str(), udp.remotePort());
        udp.read(udp_buffer, UDP_BUFFER_SIZE);
        // Serial.printf("%d: Received command with size %d sequence %d\n", millis(), packet_size, (uint8_t)udp_buffer[0]);
        // Serial.printf("%d: Received command\n", millis());
        // if (len > 0)
        // {
        //   udp_buffer[len] = 0;
        // }
        // Serial.printf("UDP packet contents: %s\n", udp_buffer);
        commands.process(udp_buffer);
        // udp.flush();
      }
      // else
      // {
      //   Serial.printf("%d: none\n", millis());
      // }
      commands.run();
    }
    else
    {
      print_wifi_status(wifi_status);
      upd_init();
    }
#endif // COMMANDS_VIA_WIFI
  }
  else if (wifi_status == WL_IDLE_STATUS || wifi_status == WL_DISCONNECTED)
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    uint8_t command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_SINGLE_COLOR, STRIP_INDEX_ALL, HUE_GREEN, 255};
    commands.process(command_buffer);
    delay(500);
  }
  else
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    uint8_t command_buffer[] = {COMMAND_BUFFER_SIZE - 1, COMMAND_SINGLE_COLOR, STRIP_INDEX_ALL, HUE_ORANGE, 255};
    commands.process(command_buffer);
    wifi_init();
  }
}