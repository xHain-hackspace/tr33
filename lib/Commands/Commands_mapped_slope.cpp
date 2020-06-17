#include <Commands.h>

void Commands::mapped_slope(LedStructure *leds, char *data)
{
  uint8_t color_index = data[0];
  float render_brightness = float(data[1]) / 255.0;
  float x1 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(data[2]) / 255.0 + MAPPING_X_MIN;
  float y1 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - data[3]) / 255.0 + MAPPING_Y_MIN;
  float x2 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(data[4]) / 255.0 + MAPPING_X_MIN;
  float y2 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - data[5]) / 255.0 + MAPPING_Y_MIN;

  // avoid infitity
  if (x1 == x2)
  {
    x2 = x2 + 0.001;
  }

  float slope = (y2 - y1) / (x2 - x1);
  float height = (x2 * y1 - x1 * y2) / (x2 - x1);

  CRGB color = ColorFromPalette(currentPalette, color_index);

  float brightness = 0;
  float distance = 0;
  float fade_distance = (MAPPING_X_MAX - MAPPING_X_MIN) * 0.1;

  bool render_full = false;
  bool render_fade = false;

  for (int i = 0; i < MAPPING_SIZE; i++)
  {
    // point (x,y) is above the line if y > slope*x + height
    distance = leds->mapping[i][3] - (slope * leds->mapping[i][2] + height);

    // render above or below the line depending on which point is first on the x axis
    render_full = false;
    render_fade = false;

    if (x1 > x2)
    {
      if (distance > fade_distance)
      {
        render_full = true;
      }
      else if (distance > 0)
      {
        render_fade = true;
      }
    }
    else
    {
      if (distance < fade_distance * -1)
      {
        render_full = true;
      }
      else if (distance < 0)
      {
        render_fade = true;
      }
    }

    if (render_full)
    {
      // full brightness
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, render_brightness);
    }
    else if (render_fade)
    {
      // fade
      brightness = Commands::ease_in_out_cubic(render_brightness * fabsf(distance / fade_distance));
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, brightness);
    }
  }
}