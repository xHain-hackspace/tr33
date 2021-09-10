#define FASTLED_ESP32_I2S true

#include <Arduino.h>
#include <FastLED.h>
#include <Commands.h>
#include <Twang.h>
#include <wifi.h>
#include <uart.h>

#ifdef LED_STRUCTURE_TR33
#include <Tr33.h>
Tr33 leds = Tr33();
#endif
#ifdef LED_STRUCTURE_DODE
Dode leds = Dode();
#endif
#ifdef LED_STRUCTURE_KELLER
#include <Keller.h>
Keller leds = Keller();
#endif
#ifdef LED_STRUCTURE_RANKEN
#include <Ranken.h>
Ranken leds = Ranken();
#endif
#ifdef LED_STRUCTURE_WAND
#include <Wand.h>
Wand leds = Wand();
#endif
#ifdef LED_STRUCTURE_TROMMEL
#include <Trommel.h>
Trommel leds = Trommel();
#endif
#ifdef LED_STRUCTURE_WOLKE
#include <Wolke.h>
Wolke leds = Wolke();
#endif
#ifdef LED_STRUCTURE_DISCOTESTRIG
#include <Discotestrig.h>
Discotestrig leds = Discotestrig();
#endif

Commands command_runner = Commands();

void setup()
{
  Serial.begin(921600);
  while (!Serial)
  {
    // do nothing
  }

  Serial.println("\n\tLED Controller\r\n");

  Serial.println("Starting up...");
  command_runner.init(&leds);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print("Initiating LED structure: ");
  leds.write_info();

#ifdef COMMANDS_VIA_WIFI
  Serial.println("Receiving Commands via WiFi. Running setup.");
  wifi_setup();
#endif
#ifdef COMMANDS_VIA_UART_PINS
  Serial.println("Receiving Commands via uart pins. Running setup.");
  uart_setup();
#endif
#ifdef COMMANDS_VIA_UART_USB
  Serial.println("Receiving Commands via uart usb. Running setup.");
  uart_setup();
#endif

  Serial.println("Startup complete, going into render loop");
}

void loop()
{

#ifndef NO_WIFI
  wifi_loop(command_runner);
#endif

#ifdef COMMANDS_VIA_UART_PINS
  uart_loop(command_runner);
#endif
#ifdef COMMANDS_VIA_UART_USB
  uart_loop(command_runner);
#endif

  command_runner.run();
}
