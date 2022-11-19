// #include <Arduino.h>
#include <Commands.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <command_schemas.pb.h>
#include <pb_decode.h>
#include <pb_encode.h>

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
uint32_t metrics_period_ms = 5 * 1000;
uint32_t last_send = 0;
const int control_port = 1337;

// FPS
uint32_t framecount = 0;
uint32_t last_metrics = 0;

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
  Serial.printf("Initializing UDP...");
  udp.begin(LISTEN_PORT);
  Serial.printf(" Done.\n Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
  Serial.printf("Using \"%s\" as control host\n", control_host);

  udp_up = true;
}

void send_metrics(Commands commands)
{
  WireMessage wm = WireMessage_init_default;
  wm.which_message = WireMessage_target_metrics_tag;
  wm.message.target_metrics = (TargetMetrics)TargetMetrics_init_default;

  // todo: write metrics inside Commands
  String name = commands.get_led_structure_name();
  name.toCharArray(wm.message.target_metrics.name, 20);
  wm.message.target_metrics.wifi_strength = WiFi.RSSI();
  String version = String(VERSION);
  version.toCharArray(wm.message.target_metrics.version, 20);
  commands.write_hashes(&wm.message.target_metrics);

  // FPS
  if (last_metrics != 0)
  {
    uint32_t fps = float(framecount) / (float(millis() - last_metrics) / 1000.0);
    wm.message.target_metrics.fps = fps;
    framecount = 0;
  }

  pb_ostream_t stream = pb_ostream_from_buffer(udp_buffer, UDP_BUFFER_SIZE);
  pb_encode(&stream, WireMessage_fields, &wm);

  udp.beginPacket(control_host, control_port);
  udp.write(udp_buffer, stream.bytes_written);
  udp.endPacket();

  last_metrics = millis();
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
  // this part is for over-the-air (OTA) updates
  // if you mess this up, you'll have to climb the ladder of shame ;)
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
                 Serial.println("Start updating " + type); })
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
                   Serial.println("End Failed"); });
  // End of OTA section
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
  Pixel pixel = Pixel_init_default;
  pixel.color = color;
  pixel.led_index = 0;
  CommandParams command = CommandParams_init_default;
  command.which_type_params = CommandParams_pixel_tag;
  command.type_params.pixel = pixel;
  command.index = COMMAND_COUNT - 1;
  // command.strip_index = 1;
  command.brightness = 100;

  return command;
}

void wifi_loop(Commands commands)
{
  int wifi_status = WiFi.status();
  int bytes;
  framecount++;

  if (wifi_status == WL_CONNECTED)
  {
    if (wasdisconnected)
    {
      // if this is a reconnect restore the previous effect
      print_wifi_status(wifi_status);
      commands.handle_command(disable_overlay());
      wasdisconnected = false;
    }
    // this part is for over-the-air (OTA) updates
    // if you mess this up, you'll have to climb the ladder of shame ;)
    if (!ota_up)
    {
      ArduinoOTA.begin();
      ota_up = true;
    }
    ArduinoOTA.handle();
    // end of OTA section

#ifdef COMMANDS_VIA_WIFI
    if (udp_up)
    {
      // regulary send metrics
      if (millis() - last_send > metrics_period_ms || last_send == 0)
      {
        send_metrics(commands);
        last_send = millis();
      }

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
            commands.handle_message(wire_message);
          }
          last_send = millis();
        }
      }
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
    delay(500);
  }
  else
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wasdisconnected = true;
    commands.handle_command(color_overlay(HUE_RED));
    wifi_init();
  }
}