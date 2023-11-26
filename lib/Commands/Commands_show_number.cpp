#include <Commands.h>

void Commands::show_number(LedStructure *leds, const CommandParams &cmd)
{
  uint8_t number = 0;

  uint8_t hundrets = number / 100;
  uint8_t tens = (number % 100) / 10;
  uint8_t ones = (number % 10);

  // hundreds - red
  for (int i = 0; i < hundrets; i++)
  {
    leds->fade_led(cmd, i, CRGB(255, 0, 0));
  }

  // tens - green
  for (int i = hundrets; i < hundrets + tens; i++)
  {
    leds->fade_led(cmd, i, CRGB(0, 255, 0));
  }

  // ones - blue
  for (int i = hundrets + tens; i < hundrets + tens + ones; i++)
  {
    leds->fade_led(cmd, i, CRGB(0, 0, 255));
  }
}