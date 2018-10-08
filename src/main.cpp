#include <Secrets.h>
#include <Arduino.h>
#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char serial_buffer[SERIAL_BUFFER_SIZE];

Commands commands = Commands();

void setup() {

  Serial.begin(921600);
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

  uint8_t size;

  Serial.println("READY");

  size = Serial.read();
  if(size) {
    Serial.readBytes(serial_buffer, size);
    commands.process((char *) serial_buffer);
    // handle the new data that came in
  }
}
