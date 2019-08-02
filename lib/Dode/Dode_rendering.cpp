#include <Dode.h>
#include <Commands.h>

void Dode::set_led(uint8_t edge_index, uint8_t led, CRGB color)
{
  leds[edges[edge_index][0]][edges[edge_index][1] + led] = color;
}

void Dode::all_off()
{
  for (int i = 0; i < HW_STRIP_COUNT; i++)
  {
    for (int j = 0; j < HW_STRIP_PIXEL_COUNT; j++)
    {
      leds[i][j] = CRGB(0, 0, 0);
    }
  }
}