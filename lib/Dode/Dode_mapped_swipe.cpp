#include <Dode.h>
#include <Commands.h>
#include <math.h>

#define MAX_NR_OF_SECTORS 255

void Dode::mapped_swipe(uint8_t *data)
{
  uint8_t color_index = data[0];
  float brightness_factor = float(data[1]) / 255.0;
  uint8_t period = data[2];
  uint8_t offset = data[3];
  float width = float(data[4]) / 20.0;
  uint8_t direction = data[5];

  float position = Commands::ping_pong_sine(period, offset) * 3.0 - 1.5;
  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  for (int i = 0; i < NR_TOTAL_LEDS; i++)
  {
    float led_position = 0.0;
    switch (direction)
    {
    case SWIPE_X:
      led_position = coordinates[i].x;
      break;
    case SWIPE_Y:
      led_position = coordinates[i].y;
      break;
    case SWIPE_Z:
      led_position = coordinates[i].z;
      break;
    case SWIPE_nX:
      led_position = -coordinates[i].x;
      break;
    case SWIPE_nY:
      led_position = -coordinates[i].y;
      break;
    case SWIPE_nZ:
      led_position = -coordinates[i].z;
      break;
    }
    float brightness = 1.0 - fabs(led_position - position) / (width / 2.0);

    if (brightness > 0)
    {
      brightness = Commands::ease_in_out_cubic(brightness * 1.0);
      fade_led(coordinates[i].i_edge, coordinates[i].i_led, color, brightness * brightness_factor * 255);
    }
  }
}
