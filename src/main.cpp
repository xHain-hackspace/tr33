#include <Arduino.h>
#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char serial_buffer[SERIAL_BUFFER_SIZE];

Commands commands = Commands();

HardwareSerial DebugSerial(0);
HardwareSerial CommandSerial(0);

void setup() {
  Serial.begin(230400);
  DebugSerial.begin(230400);
  while (!DebugSerial) {}

  CommandSerial.begin(230400);
  while (!CommandSerial) {}

  DebugSerial.println("Starting up...");

  commands.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  DebugSerial.println("Startup complete");
}


void loop() {
  int size;

  // while(CommandSerial.available()) {
  //   CommandSerial.read();
  // }

  // CommandSerial.println("OK");
  // CommandSerial.readBytes(serial_buffer, 1);
  // size = serial_buffer[0];

  // if(size > 0) {
  //   CommandSerial.readBytes(serial_buffer, size);
  //   commands.process((char *) serial_buffer);
  // }

  commands.run();
}
