#include <Arduino.h>
#include <FastLED.h>
#include <Commands.h>
#include <Tr33.h>
#include <Dode.h>

#define SERIAL_HEADER 42
#define SERIAL_READY_TO_SEND 0xAA
#define SERIAL_CLEAR_TO_SEND 0xBB
#define SERIAL_REQUEST_RESYNC 0xCC

const uint8_t SERIAL_BUFFER_SIZE = 1024;
char serial_buffer[SERIAL_BUFFER_SIZE];
const uint8_t SERIAL_PACKET_SIZE = 2 + COMMAND_DATA_SIZE;
const uint8_t SERIAL_TIMEOUT = 100;

Commands commands;

HardwareSerial CommandSerial(2);
// HardwareSerial CommandSerial(0);

// Tr33 led_structure = Tr33();
Dode led_structure = Dode();

void flush_serial()
{
  while (CommandSerial.available())
  {
    int byte = CommandSerial.read();
    Serial.print("Flushing from serial: ");
    Serial.println(byte);
  }
}

void setup()
{
  Serial.begin(230400);
  CommandSerial.begin(921600);
  while (!Serial || !CommandSerial)
  {
    // do nothing
  }

  CommandSerial.setTimeout(SERIAL_TIMEOUT);

  Serial.println("Starting up...");

  commands.init(&led_structure);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");

  CommandSerial.write(SERIAL_REQUEST_RESYNC);
}

void loop()
{
  int byte;
  int commandCount;
  int bytes_read;

  byte = CommandSerial.read();

  if (byte == SERIAL_READY_TO_SEND)
  {
    CommandSerial.write(SERIAL_CLEAR_TO_SEND);
    long cts_send_time = millis();

    while (byte != SERIAL_HEADER && millis() < cts_send_time + SERIAL_TIMEOUT)
    {
      byte = CommandSerial.read();
    }

    if (byte == SERIAL_HEADER)
    {
      commandCount = CommandSerial.read();

      for (int i = 0; i < commandCount; i++)
      {
        bytes_read = CommandSerial.readBytes(serial_buffer, SERIAL_PACKET_SIZE);
        if (bytes_read == SERIAL_PACKET_SIZE)
        {
          commands.process((char *)serial_buffer);
        }
        else
        {
          Serial.print("Incomplete message, bytes read: ");
          Serial.println(bytes_read);
        }
      }
    }
    else
    {
      // Serial.print("Expected header, got: ");
      // Serial.println(byte);
    }
  }
  else if (byte != -1)
  {
    // Serial.print("Expected RTS, got: ");
    // Serial.println(byte);
    // flush_serial();
    // CommandSerial.write(SERIAL_REQUEST_RESYNC);
  }

  commands.run();
}
