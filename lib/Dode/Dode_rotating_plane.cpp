#include <Dode.h>
#include <Commands.h>
#include <math.h>

#define MAX_NR_OF_SECTORS 255

void Dode::rotating_plane(uint8_t *data)
{
  uint8_t color_index = data[0];
  float plane_brightness = float(data[1]) / 255;
  float radius = float(data[2]) / 255.0 * 2.0;
  float phi = float(data[3]) / 255.0 * 2.0 * PI;
  float theta = float(data[4]) / 255.0 * PI;
  float width = float(data[5]) / 20.0;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);
  float distance = 0.0;
  float brightness = 1.0;

  for (int i = 0; i < NR_TOTAL_LEDS; i++)
  {
    distance = sqrtf(sq(radius) + sq(coordinates[i].r) - 2.0 * radius * coordinates[i].r * (sinf(coordinates[i].theta) * sinf(theta) * cosf(phi - coordinates[i].phi) + cosf(coordinates[i].theta) * cosf(theta)));
    if (distance < width)
    {
      brightness = Commands::ease_in_out_cubic(plane_brightness * 1.0 * ((width - distance) / width));
      fade_led(coordinates[i].i_edge, coordinates[i].i_led, color, brightness);
    }
  }
}
