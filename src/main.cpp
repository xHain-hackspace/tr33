#define FASTLED_ESP32_I2S true

#include <Arduino.h>
#include <FastLED.h>
#include <Commands.h>
#include <Twang.h>
#include <Network.h>
#include <ColorTools.h>

#ifdef LED_STRUCTURE_TR33
#include <Tr33.h>
Tr33 leds = Tr33();
#endif
#ifdef LED_STRUCTURE_DODE
Dode leds = Dode();
#endif
#ifdef LED_STRUCTURE_RANKEN
#include <Ranken.h>
Ranken leds = Ranken();
#endif
#ifdef LED_STRUCTURE_WAND
#include <Wand.h>
Wand leds = Wand();
#endif
#ifdef LED_STRUCTURE_NEON
#include <Neon.h>
Neon leds = Neon();
#endif
#ifdef LED_STRUCTURE_TROMMEL
#include <Trommel.h>
Trommel leds = Trommel();
#endif
#ifdef LED_STRUCTURE_WOLKE
#include <Wolke.h>
Wolke leds = Wolke();
#endif
#ifdef LED_STRUCTURE_SCUBAR
#include <Scubar.h>
Scubar leds;
#endif
#ifdef LED_STRUCTURE_TETRAEDER
#include <Tetraeder.h>
Tetraeder leds;
#endif
#ifdef LED_STRUCTURE_ARROW
#include <Arrow.h>
Arrow leds;
#endif

Commands command_runner = Commands();

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // do nothing
  }

  Serial.println("\n\tLED Controller\r\n");

  Serial.println("Starting up...");
  command_runner.init(&leds);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.printf("Running on LED structure: %s\n", leds.get_name());

  Serial.println("Receiving Commands via WiFi. Running setup.");
  Network::setup();

  Serial.println("Startup complete, going into render loop");
}

void loop()
{
  Network::loop(command_runner);
  command_runner.run();
}
