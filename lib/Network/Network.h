#ifndef __NETWORK_H_INCLUDED__
#define __NETWORK_H_INCLUDED__

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Commands.h>

#define UDP_BUFFER_SIZE 1500
#define LISTEN_PORT 1337
#define SEND_PORT 1337
#define METRICS_INTERVAL 5 * 1000
#define OTA_PASSWORD_HASH "d3d57181ad9b5b2e5e82a6c0b94ba22e"

class Network
{
private:
  static void udp_setup();
  static void ota_setup();
  static void print_wifi_status(wl_status_t status);
  static void send_color_palette_response(WireMessage wire_message);
  static void send_metrics(Commands commands);
  static void send_udp_packet(WireMessage wire_message);

public:
  static void setup();
  static void loop(Commands commands);
  static void remote_log(String message);
};

#endif // __NETWORK_H_INCLUDED__
