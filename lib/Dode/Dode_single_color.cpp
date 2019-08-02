#include <Dode.h>
#include <Commands.h>

void Dode::single_color(char *data)
{
  uint8_t color_index = data[0];
  uint8_t brightness = data[1];

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    for (int j = 0; j < EDGE_PIXEL_COUNT; j++)
    {
      set_led(i, j, ColorFromPalette(currentPalette, color_index, brightness));
    }
  }
}