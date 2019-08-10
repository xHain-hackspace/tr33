#include <Commands.h>

void Commands::single_color(Leds *leds, char *data)
{
  uint8_t strip_index = data[0];
  uint8_t color_index = data[1];
  uint8_t brightness = data[2];

  for (int i = 0; i < leds->strip_length(strip_index); i++)
  {
    leds->set_led(strip_index, i, ColorFromPalette(currentPalette, color_index, brightness));
  }
}