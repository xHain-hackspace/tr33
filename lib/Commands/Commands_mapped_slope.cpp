#include <Commands.h>

void Commands::mapped_ping_pong(LedStructure *leds, CommandParams cmd)
{
}
void Commands::mapped_slope(LedStructure *leds, CommandParams cmd)
{
  MappedSlope mapped_slope = cmd.type_params.mapped_slope;

  float render_brightness = float(cmd.brightness) / 255.0;
  float x1 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_slope.x1) / 255.0 + MAPPING_X_MIN;
  float y1 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_slope.y1) / 255.0 + MAPPING_Y_MIN;
  float x2 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_slope.x2) / 255.0 + MAPPING_X_MIN;
  float y2 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_slope.y2) / 255.0 + MAPPING_Y_MIN;
  float fade_distance = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(mapped_slope.fade_distance) / 255.0 / 0.5;

  // avoid infitity
  if (x1 == x2)
  {
    x2 = x2 + 0.001;
  }

  float slope = (y2 - y1) / (x2 - x1);
  float height = (x2 * y1 - x1 * y2) / (x2 - x1);

  CRGB color = color_from_palette(cmd, mapped_slope.color);

  float brightness = 0;
  float distance = 0;

  bool render_full = false;
  bool render_fade = false;

  for (int i = 0; i < MAPPING_SIZE; i++)
  {
    // point (x,y) is above the line if y > slope*x + height
    distance = leds->mapping[i][3] - (slope * leds->mapping[i][2] + height);

    // render above or below the line depending on which point is first on the x axis
    render_full = false;
    render_fade = false;

    if (mapped_slope.slope_type == SlopeType_LINE) // || type == SLOPE_LINE_INVERSE)
    {
      if (fabs(distance) < fade_distance)
      {
        render_fade = true;
      }
    }
    else if (mapped_slope.slope_type == SlopeType_FILL)
    {
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
    }

    if (render_full)
    {
      // full brightness
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, render_brightness);
    }
    else if (render_fade)
    {
      // fade
      if (mapped_slope.slope_type == SlopeType_FILL) // || type == SLOPE_LINE_INVERSE)
      {
        brightness = Commands::ease_in_out_cubic(render_brightness * fabsf(distance / fade_distance));
      }
      else if (mapped_slope.slope_type == SlopeType_LINE)
      {
        brightness = Commands::ease_in_out_cubic(render_brightness * (1 - fabsf(distance / fade_distance)));
      }
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, brightness);
    }
  }
}