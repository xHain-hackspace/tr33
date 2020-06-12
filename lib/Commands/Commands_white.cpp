#include <Commands.h>

void Commands::white(LedStructure *leds, char *data)
{
  for (int i = 0; i < leds->strip_length(leds->strip_index_all()); i++)
  {
    leds->set_led(leds->strip_index_all(), i, CRGB(255, 255, 255));
  }
}