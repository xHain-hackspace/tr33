#include <Network.h>
#include <FastLED.h>
#include <Commands.h>
#include <LedStructure.h>
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

#if defined(WIFI_HOSTNAME)
char hostname[] = WIFI_HOSTNAME;
#else
char hostname[] = "esp32";
#endif

uint8_t udp_buffer[UDP_BUFFER_SIZE];
WiFiUDP udp;
bool udp_up;
bool ota_up;
uint8_t wifi_error_count;
uint32_t last_metrics;
uint32_t fps_framecount;

void Network::setup()
{
  WiFi.setHostname(hostname);

  Serial.printf("Initializing WiFi...\n");

  delay(10);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE); // needs to be here for the DHCP hostname to work...

  Serial.printf("Connecting to ssid %s, setting hostname to %s\n", ssid, hostname);

  if (!WiFi.setHostname(hostname))
  {
    Serial.printf("Failed to set hostname to %s\n", hostname);
  };

  Serial.printf("Wlan begin... ");
  WiFi.begin(ssid, password);

  ota_setup();
  wifi_error_count = 0;
}

void Network::loop(Commands commands)
{
  wl_status_t wifi_status = WiFi.status();
  int bytes_read;
  fps_framecount++;

  if (wifi_status == WL_CONNECTED)
  {
    wifi_error_count = 0;

    if (!ota_up)

    {
      ArduinoOTA.begin();
      ota_up = true;
    }
    ArduinoOTA.handle();

    if (udp_up)
    {
      // regulary send metrics
      if (millis() - last_metrics > METRICS_INTERVAL || last_metrics == 0)
      {
        send_metrics(commands);
        last_metrics = millis();
      }

      // wifi and upd is up, check if we received a UDP packet
      while (udp.parsePacket())
      {
        bytes_read = udp.read(udp_buffer, UDP_BUFFER_SIZE);

        if (bytes_read > 0)
        {
          // try to decode packet as protobuf
          pb_istream_t stream = pb_istream_from_buffer(udp_buffer, bytes_read);
          WireMessage wire_message = WireMessage_init_default;
          bool status = pb_decode(&stream, WireMessage_fields, &wire_message);

          if (!status)
          {
            remote_log("Protobuf decoding failed: " + String(PB_GET_ERROR(&stream)));
          }
          else if (wire_message.which_message == WireMessage_color_palette_request_tag)
          {
            Serial.printf("Received color_palette_request\n", PB_GET_ERROR(&stream));
            send_color_palette_response(wire_message);
          }
          else
          {
            commands.handle_message(wire_message);
          }
        }
      }
    }
    else
    {
      print_wifi_status(wifi_status);
      udp_setup();
    }
  }
  else if (wifi_status == WL_IDLE_STATUS || wifi_status == WL_DISCONNECTED)
  {
    print_wifi_status(wifi_status);
    udp_up = false;
    ota_up = false;
    wifi_error_count++;
    if (wifi_error_count > 150)
    {
      ESP.restart();
    }
  }
  else
  {
    print_wifi_status(wifi_status);
    ESP.restart();
  }
}

void Network::udp_setup()
{
  Serial.printf("Initializing UDP...");

  if (udp.begin(LISTEN_PORT))
  {
    Serial.printf(" Done.\n Listening on %s:%d\n", WiFi.localIP().toString().c_str(), LISTEN_PORT);
    Serial.printf("Using \"%s\" as control host\n", control_host);

    udp_up = true;
  }
  else
  {
    Serial.printf(" Failed. Could not open Port.\n");
  }
}

void Network::ota_setup()
{
  // this part is for over-the-air (OTA) updates
  // if you mess this up, you'll have to climb the ladder of shame ;)
  Serial.println("Setting up OTA");
  ArduinoOTA.setPasswordHash(OTA_PASSWORD_HASH);
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
}

void Network::send_udp_packet(WireMessage wire_message)
{
  pb_ostream_t stream = pb_ostream_from_buffer(udp_buffer, UDP_BUFFER_SIZE);
  pb_encode(&stream, WireMessage_fields, &wire_message);

  udp.beginPacket(control_host, SEND_PORT);
  udp.write(udp_buffer, stream.bytes_written);
  udp.endPacket();
}

void Network::send_metrics(Commands commands)
{
  WireMessage wm = WireMessage_init_default;
  wm.which_message = WireMessage_target_metrics_tag;
  wm.message.target_metrics = (TargetMetrics)TargetMetrics_init_default;

  // todo: write metrics inside Commands
  String name = commands.leds->get_name();
  name.toCharArray(wm.message.target_metrics.name, 20);
  wm.message.target_metrics.wifi_strength = WiFi.RSSI();
  String version = String(VERSION);
  version.toCharArray(wm.message.target_metrics.version, 20);
  commands.write_hashes(&wm.message.target_metrics);
  wm.message.target_metrics.max_strip_index = commands.leds->get_max_strip_index();
  wm.message.target_metrics.free_heap = ESP.getFreeHeap();
  wm.message.target_metrics.heap_size = ESP.getHeapSize();
  wm.message.target_metrics.cpu_freq_mhz = ESP.getCpuFreqMHz();
  wm.message.target_metrics.chip_revision = ESP.getChipRevision();
  strncpy(wm.message.target_metrics.chip_model, ESP.getChipModel(), 20);
  strncpy(wm.message.target_metrics.sdk_version, ESP.getSdkVersion(), 20);

  // FPS
  if (last_metrics != 0)
  {
    uint32_t fps = float(fps_framecount) / (float(millis() - last_metrics) / 1000.0);
    wm.message.target_metrics.fps = fps;
    fps_framecount = 0;
  }

  send_udp_packet(wm);
}

void Network::remote_log(String message)
{
  Serial.println("Remote log: " + message);

  WireMessage wm = WireMessage_init_default;
  wm.which_message = WireMessage_remote_log_tag;
  wm.message.remote_log = (RemoteLog)RemoteLog_init_default;

  message.toCharArray(wm.message.remote_log.message, 100);
  send_udp_packet(wm);
}

void Network::send_color_palette_response(WireMessage wire_message)
{
  Serial.printf("Creating color_palette_response\n");

  WireMessage wm = WireMessage_init_default;
  wm.which_message = WireMessage_color_palette_response_tag;
  wm.message.color_palette_response = (ColorPaletteResponse)ColorPaletteResponse_init_default;

  wm.message.color_palette_response.color_palette = wire_message.message.color_palette_request.color_palette;

  CRGB color = CRGB::Black;

  for (uint16_t i = 0; i < 256; i++)
  {
    color = Commands::color_from_palette(wire_message.message.color_palette_request.color_palette, i, 255);
    wm.message.color_palette_response.colors[i] = color.r << 16 | color.g << 8 | color.b;
  }
  send_udp_packet(wm);
}

void Network::print_wifi_status(wl_status_t wifi_status)
{
  if (wifi_status == WL_CONNECTED)
  {
    Serial.printf("WiFi status: WL_CONNECTED\n");
    Serial.printf("IP:  %s\n", WiFi.localIP().toString().c_str());
  }
  else if (wifi_status == WL_NO_SHIELD)
  {
    Serial.printf("WiFi status: WL_NO_SHIELD, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_IDLE_STATUS)
  {
    Serial.printf("WiFi status: WL_IDLE_STATUS, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_NO_SSID_AVAIL)
  {
    Serial.printf("WiFi status: WL_NO_SSID_AVAIL, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_SCAN_COMPLETED)
  {
    Serial.printf("WiFi status: WL_SCAN_COMPLETED, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_CONNECT_FAILED)
  {
    Serial.printf("WiFi status: WL_CONNECT_FAILED, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_CONNECTION_LOST)
  {
    Serial.printf("WiFi status: WL_CONNECTION_LOST, error_count: %d\n", wifi_error_count);
  }
  else if (wifi_status == WL_DISCONNECTED)
  {
    Serial.printf("WiFi status: WL_DISCONNECTED, error_count: %d\n", wifi_error_count);
  }
  else
  {
    Serial.printf("WiFi status: %d\n", wifi_status);
  }
}