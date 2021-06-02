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
  int status = WiFi.begin(ssid, password);
  Serial.printf("Wlan begin status %i\n", status);
}

void upd_init()
{
  Serial.printf("Initializing UDP...\n");
  udp.begin(LISTEN_PORT);
  Serial.printf("Done. Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
  // send resync request
  Serial.printf("Sending resync request to %s:%d\n", resync_host, resync_port);
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
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";

                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type);
               })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
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

CommandParams disable_overlay()
{
  CommandParams command = CommandParams_init_zero;
  command.index = COMMAND_COUNT - 1;
  command.enabled = false;

  return command;
}

CommandParams color_overlay(uint8_t color)
{
  SingleColor single = SingleColor_init_default;
  single.color = color;
  CommandParams command = CommandParams_init_default;
  command.which_type_params = CommandParams_single_color_tag;
  command.type_params.single_color = single;
  command.index = COMMAND_COUNT - 1;

  return command;
}

void wifi_loop(Commands commands)
{
  int wifi_status = WiFi.status();

  if (wifi_status == WL_CONNECTED)
  {
    if (wasdisconnected)
    {
      //if this is a reconnect restore the previous effect
      print_wifi_status(wifi_status);
      commands.process(disable_overlay());
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
        int bytes = udp.read(udp_buffer, UDP_BUFFER_SIZE);
        commands.process(udp_buffer, bytes);
      }
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
    commands.process(color_overlay(HUE_GREEN));
    commands.run();
    delay(500);
  }
  else
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    commands.process(color_overlay(HUE_ORANGE));
    commands.run();
    wifi_init();
  }
}