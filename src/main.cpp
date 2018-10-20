#include <Arduino.h>
#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char serial_buffer[SERIAL_BUFFER_SIZE];

Commands commands = Commands();

void setup() {

  Serial.begin(230400);
  while (!Serial) {}

  Serial2.begin(230400);
  while (!Serial2) {}

  Serial.println("Starting up...");

  commands.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");
}


void loop() {
  int size;

  while(Serial2.available()) {
    int byte = Serial2.read();
    // Serial.println("Cleared 1 Byte from serial buffer");
    // Serial.println(byte);
  }

  Serial2.println("OK");
  Serial2.readBytes(serial_buffer, 1);
  size = serial_buffer[0];

  // Serial.print("Command size:");
  // Serial.println(size);
  if(size > 0) {
    Serial2.readBytes(serial_buffer, size);
    // Serial.println("Command read: ");
    // Serial.println(serial_buffer);
    commands.process((char *) serial_buffer);
    // Serial.println("Command processed");
  }

  commands.run();
}
