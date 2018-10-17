#include <Arduino.h>
#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char serial_buffer[SERIAL_BUFFER_SIZE];

Commands commands = Commands();

void setup() {

  Serial.begin(230400);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Starting up...");

  commands.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");
}


void loop() {
  int size;

  while(Serial.available()) {
    int byte = Serial.read();
    // Serial.println("Cleared 1 Byte from serial buffer");
    // Serial.println(byte);
  }

  Serial.println("OK");
  Serial.readBytes(serial_buffer, 1);
  size = serial_buffer[0];

  // Serial.print("Command size:");
  // Serial.println(size);
  if(size > 0) {
    Serial.readBytes(serial_buffer, size);
    // Serial.println("Command read: ");
    // Serial.println(serial_buffer);
    commands.process((char *) serial_buffer);
    // Serial.println("Command processed");
  }

  commands.run();
}
