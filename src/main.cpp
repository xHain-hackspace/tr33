#include <Secrets.h>
#include <Arduino.h>
#include <Commands.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// wifi settings
IPAddress ip(192, 168, 0, 42);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// udp settings
#define UDP_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char udp_buffer[UDP_BUFFER_SIZE];
WiFiUDP udp;
bool udp_up = false;

Commands commands = Commands();

void setup() {
  delay(1000);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Starting up...");

  commands.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");
}

void wifi_init() {
  WiFi.disconnect(true);
  Serial.printf("Connecting to ssid %s\n", ssid);
  if (!WiFi.config(ip, gateway, subnet)) {
    Serial.println("Wifi config failed!");
  }
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

void loop() {
  int wifi_status = WiFi.status();

  if(wifi_status == WL_CONNECTED) {
    if (udp_up) {
      // wifi and upd is up, check if we received a UDP packet
      int packet_size = udp.parsePacket();
      if (packet_size) {
        // Serial.printf("Received command with size %d from %s, port %d\n", packet_size, udp.remoteIP().toString().c_str(), udp.remotePort());
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
    commands.start_sequence();
    delay(500);
  } else {
    print_wifi_status(wifi_status);
    udp_up = false;
    wifi_init();
    commands.start_sequence();
  }
}
