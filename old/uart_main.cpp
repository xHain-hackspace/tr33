#include <Arduino.h>
#include <Commands.h>
#include <uart_main.h>
#include <command_schemas.pb.h>
#include <pb_decode.h>

#define SERIAL_HEADER 42
#define SERIAL_READY_TO_SEND 0xAA
#define SERIAL_CLEAR_TO_SEND 0xBB
#define SERIAL_REQUEST_RESYNC 0xCC

const uint16_t SERIAL_BUFFER_SIZE = 1024;
uint8_t serial_buffer[SERIAL_BUFFER_SIZE];
const uint8_t SERIAL_TIMEOUT = 100;

#ifdef COMMANDS_VIA_UART_PINS
HardwareSerial CommandSerial(2);
#endif
#ifdef COMMANDS_VIA_UART_USB
HardwareSerial CommandSerial(0);
#endif
#ifdef COMMANDS_VIA_WIFI
HardwareSerial CommandSerial(0);
#endif

void flush_serial()
{
  while (CommandSerial.available())
  {
    int byte = CommandSerial.read();
    Serial.print("Flushing from serial: ");
    Serial.println(byte);
  }
}

void uart_loop(Commands commands)
{
  int byte;
  int byte_count;
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
      byte_count = CommandSerial.read();
      bytes_read = CommandSerial.readBytes(serial_buffer, byte_count);
      if (bytes_read == byte_count)
      {

        pb_istream_t stream = pb_istream_from_buffer(serial_buffer, bytes_read);
        WireMessage wire_message = WireMessage_init_default;
        bool status = pb_decode(&stream, WireMessage_fields, &wire_message);

        if (!status)
        {
          Serial.printf("Protobuf decoding failed: %s\n", PB_GET_ERROR(&stream));
        }
        else
        {
          commands.handle_message(wire_message);
        }
      }
      else
      {
        Serial.printf("Incomplete message, bytes read: %i. Expecting %i\n", bytes_read, byte_count);
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
    Serial.printf("Expected %i, got: %i \n ", SERIAL_READY_TO_SEND, byte);
    Serial.println("Flushing serial");
    flush_serial();
    // CommandSerial.write(SERIAL_REQUEST_RESYNC);
  }

  commands.run();
}

void uart_setup()
{
  CommandSerial.begin(921600);
  while (!CommandSerial)
  {
    // do nothing
  }
  CommandSerial.setTimeout(SERIAL_TIMEOUT);
  Serial.println("UART: Sending resync request");

  CommandSerial.write(SERIAL_REQUEST_RESYNC);
}
