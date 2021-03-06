#include <Arduino.h>
#include <Commands.h>
// #include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <command_schemas.pb.h>
#include <pb_decode.h>

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

// syncing
uint32_t sequence_period_ms = 20 * 1000;
uint32_t last_send = 0;
uint8_t sequence = 0;
bool sequence_error = false;
const int control_port = 1337;
const int sequence_header_length = 3;
const char sequence_header[] = "SEQ";

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
  int status = WiFi.begin(ssid, password);
  Serial.printf("Wlan begin status %i\n", status);
}

void upd_init()
{
  Serial.printf("Initializing UDP...\n");
  udp.begin(LISTEN_PORT);
  Serial.printf("Done. Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
  Serial.println("... done");

  udp_up = true;
}

void send_sequence()
{
  udp.beginPacket(control_host, control_port);
  udp.write((uint8_t *)sequence_header, sequence_header_length);
  if (sequence_error)
  {
    udp.write(0);
  }
  else
  {
    udp.write(sequence);
  }
  sequence_error = false;
  udp.endPacket();
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
  int bytes;

  if (wifi_status == WL_CONNECTED)
  {
    if (wasdisconnected)
    {
      //if this is a reconnect restore the previous effect
      print_wifi_status(wifi_status);
      commands.handle_command(disable_overlay());
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
        bytes = udp.read(udp_buffer, UDP_BUFFER_SIZE);

        if (bytes > 0)
        {
          pb_istream_t stream = pb_istream_from_buffer(udp_buffer, bytes);
          WireMessage wire_message = WireMessage_init_default;
          bool status = pb_decode(&stream, WireMessage_fields, &wire_message);

          if (!status)
          {
            Serial.printf("Protobuf decoding failed: %s\n", PB_GET_ERROR(&stream));
          }
          else
          {
            if ((sequence + 1) % 256 != wire_message.sequence)
            {
              sequence_error = true;
            }

            sequence = wire_message.sequence;
            commands.handle_message(wire_message);
          }
          last_send = millis();
        }
      }

      // regulary send squence to ensure we are in sync
      if (millis() - last_send > sequence_period_ms || last_send == 0)
      {
        send_sequence();
        last_send = millis();
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
    commands.handle_command(color_overlay(HUE_ORANGE));
    commands.run();
    delay(500);
  }
  else
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    commands.handle_command(color_overlay(HUE_RED));
    commands.run();
    wifi_init();
  }
}