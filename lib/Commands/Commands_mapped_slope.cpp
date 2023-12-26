#include <Commands.h>

void Commands::mapped_slope(LedStructure *leds, const CommandParams &cmd)
{
  MappedSlope mapped_slope = cmd.type_params.mapped_slope;

  float brightness = float(cmd.brightness) / 255.0f;
  float x1 = float(mapped_slope.x1) * 8.0f / 255.0f;
  float y1 = float(255 - mapped_slope.y1) * 8.0f / 255.0f;
  float x2 = float(mapped_slope.x2) * 8.0f / 255.0f;
  float y2 = float(255 - mapped_slope.y2) * 8.0f / 255.0f;

  float fade_distance = float(mapped_slope.fade_distance) / 255.0f;
  CRGB color = color_from_palette(cmd, mapped_slope.color);

  render_mappled_slope(leds, x1, y1, x2, y2, mapped_slope.slope_type, fade_distance, color, brightness);
}

void Commands::render_mappled_slope(LedStructure *leds, float x1, float y1, float x2, float y2, SlopeType slope_type, float fade_distance, CRGB color, float brightness)
{
  // avoid infitity
  if (x1 == x2)
  {
    x2 = x2 + 0.001;
  }

  float slope = (y2 - y1) / (x2 - x1);
  float height = (x2 * y1 - x1 * y2) / (x2 - x1);

  CRGB target_color = color;

  float current_brightness = 0;
  float distance = 0;

  bool render_full = false;
  bool render_fade = false;

  for (int i = 0; i < leds->mapping_size(); i++)
  {
    // point (x,y) is above the line if y > slope*x + height
    distance = leds->mapping_y(i) - (slope * leds->mapping_x(i) + height);

    // render above or below the line depending on which point is first on the x axis
    render_full = false;
    render_fade = false;

    if (slope_type == SlopeType_LINE)
    {
      if (fabs(distance) < fade_distance)
      {
        render_fade = true;
      }
    }
    else if (slope_type == SlopeType_FILL || slope_type == SlopeType_COLOR_SHIFT)
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

    current_brightness = 0;
    if (render_full)
    {
      current_brightness = brightness;
    }
    else if (render_fade)
    {
      // fade
      if (slope_type == SlopeType_FILL) // || type == SLOPE_LINE_INVERSE)
      {
        current_brightness = Commands::ease_in_out_cubic(brightness * fabsf(distance / fade_distance));
      }
      else if (slope_type == SlopeType_LINE)
      {
        current_brightness = Commands::ease_in_out_cubic(brightness * (1 - fabsf(distance / fade_distance)));
      }
    }

    if (current_brightness > 0)
    {
      if (slope_type == SlopeType_COLOR_SHIFT)
      {
        leds->invert_led(leds->mapping_sprip_index(i), leds->mapping_led(i), current_brightness * 255);
      }
      else
      {
        leds->fade_led(leds->mapping_sprip_index(i), leds->mapping_led(i), target_color, current_brightness * 255);
      }
    }
  }
}