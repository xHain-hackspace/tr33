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

Commands commands = Commands();

void setup() {
  delay(1000);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Starting up...");

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("WiFi connected to %s\n", ssid);
  Serial.println(WiFi.localIP());

  udp.begin(LISTEN_PORT);
  Serial.printf("Listening on port %d\n", LISTEN_PORT);

  commands.initial();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");
}

void loop() {
  // check if we received a UDP packet
  int packet_size = udp.parsePacket();
  if (packet_size) {
    Serial.printf("Received command with size %d from %s, port %d\n", packet_size, udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(udp_buffer, UDP_BUFFER_SIZE);
    if (len > 0) {
      udp_buffer[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", udp_buffer);

    commands.process((char *) udp_buffer);
  }

  commands.run();
}
