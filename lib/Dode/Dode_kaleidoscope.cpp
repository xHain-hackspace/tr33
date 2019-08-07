#include <Dode.h>
#include <Commands.h>

void Dode::kaleidoscope(char *data)
{
  uint8_t color_index = data[0];
  uint8_t brightness = data[1];
  uint8_t period = data[2];

  float current_width = Commands::ping_pong_linear(period) * EDGE_PIXEL_COUNT / 2;
  CRGB color = ColorFromPalette(currentPalette, color_index, brightness);

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    for (int j = 0; j < EDGE_PIXEL_COUNT / 2; j++)
    {
      if (j <= current_width)
      {
        set_led_mirrored_middle(i, j, color);
      }
    }
  }
}