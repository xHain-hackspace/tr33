#include <Arduino.h>
#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
#define LISTEN_PORT 1337
char serial_buffer[SERIAL_BUFFER_SIZE];

Commands commands = Commands();

void setup() {

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


void loop() {
  int size;
  Serial.println("READY");
  size = Serial.read();
  Serial.print("Command size:");
  Serial.println(size);
  if(size > 0) {
    Serial.readBytes(serial_buffer, size);
    Serial.print("Command read: ");
    Serial.println(serial_buffer);
    commands.process((char *) serial_buffer);
    Serial.println("Command processed");
  }

  commands.run();
}
