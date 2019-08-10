#include <Tr33.h>
#include <Commands.h>

void Tr33::single_color(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t color_index = data[1];
  uint8_t brightness = data[2];

  for (int i = 0; i < strip_length(strip_index); i++)
  {
    set_led(strip_index, i, ColorFromPalette(currentPalette, color_index, brightness));
  }
}