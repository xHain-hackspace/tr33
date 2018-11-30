#include <Commands.h>

#define SERIAL_BUFFER_SIZE 256
char serial_buffer[SERIAL_BUFFER_SIZE];
int serial_packet_size = 3 + COMMAND_DATA_SIZE;

Commands commands = Commands();

HardwareSerial CommandSerial(0);

void flush_serial() {
  while(CommandSerial.available()) {
    CommandSerial.read();
  }
}

void setup() {
  Serial.begin(230400);
  while (!Serial) {}

  CommandSerial.begin(230400);
  while (!CommandSerial) {}
  CommandSerial.setTimeout(100);

  Serial.println("Starting up...");

  commands.init();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Startup complete");

  CommandSerial.write(SERIAL_REQUEST_RESYNC);
}


void loop() {
  int maybe_rts = Serial.read();  

  if(maybe_rts == SERIAL_READY_TO_SEND) {
    flush_serial();    
    CommandSerial.write(SERIAL_CLEAR_TO_SEND);
    int bytes_read = CommandSerial.readBytes(serial_buffer, serial_packet_size);
    if (bytes_read == serial_packet_size && serial_buffer[0] == SERIAL_HEADER) {      
      commands.process((char *) &serial_buffer[1]);
    } else {
      Serial.println("Invalid header or incomplete message");
      flush_serial();
      CommandSerial.print(SERIAL_REQUEST_RESYNC);
    }
  } else if (maybe_rts != -1){
    Serial.println("Expected RTS");
    flush_serial();
    CommandSerial.print(SERIAL_REQUEST_RESYNC);
  }

  commands.run();
}
