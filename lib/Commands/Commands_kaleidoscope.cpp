#include <Commands.h>

void Commands::kaleidoscope(LedStructure *leds, char *data)
{
  uint8_t strip_index = data[0];

  uint16_t length = leds->strip_length(strip_index);
  
  for (int i = 0; i < length / 2; i++)
  {
    leds->set_led(strip_index, length - i - 1, leds->get_led(strip_index, i));
  }
}
