#include <Commands.h>

float distance_to_square(float pixel[4], float x, float y, float size)
{
  float x_max = x + size / 2.0;
  float x_min = x - size / 2.0;
  float y_max = y + size / 2.0;
  float y_min = y - size / 2.0;

  float distance_x = max(pixel[2] - x_max, x_min - pixel[2]);
  float distance_y = max(pixel[3] - y_max, y_min - pixel[3]);

  if (distance_x <= 0 && distance_y <= 0)
  {
    return max(distance_x, distance_y);
  }
  else if (distance_x <= 0)
  {
    return distance_y;
  }
  else if (distance_y <= 0)
  {
    return distance_x;
  }
  else
  {
    return sqrtf(distance_x * distance_x + distance_y * distance_y);
  }
}

float distance_to_circle(float pixel[4], float x, float y, float size)
{
  float distance_x = x - pixel[2];
  float distance_y = y - pixel[3];
  return sqrtf(distance_x * distance_x + distance_y * distance_y) - size;
}

void Commands::mapped_render_ball(LedStructure *leds, float x, float y, float size, CRGB color, float render_brightness, float fade_distance)
{
  float distance, brightness;
  for (int i = 0; i < MAPPING_SIZE; i++)
  {
    distance = distance_to_circle(leds->mapping[i], x, y, size);

    if (distance < 0)
    {
      // full brightness
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, render_brightness * 255);
    }
    else if (distance < fade_distance)
    {
      // fade
      brightness = Commands::ease_in_out_cubic(render_brightness * (1 - distance / fade_distance));
      leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, brightness * 255);
    }
  }
}

void Commands::mapped_shape(LedStructure *leds, CommandParams cmd)
{
  MappedShape mapped_shape = cmd.type_params.mapped_shape;

  float render_brightness = float(cmd.brightness) / 255.0;
  float x = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_shape.x) / 255.0 + MAPPING_X_MIN;
  float y = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_shape.y) / 255.0 + MAPPING_Y_MIN;
  float size = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(mapped_shape.size) / 255.0;
  float fade_distance = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(mapped_shape.fade_distance) * 0.3 / 255.0;

  CRGB color = color_from_palette(cmd, mapped_shape.color);
  float brightness = 0;
  float distance = 0;

  Serial.println(render_brightness);

  if (mapped_shape.shape == Shape2D_CIRCLE)
  {
    mapped_render_ball(leds, x, y, size, color, render_brightness, fade_distance);
  }
  else
  {
    for (int i = 0; i < MAPPING_SIZE; i++)
    {
      switch (mapped_shape.shape)
      {
      case Shape2D_SQUARE:
        distance = distance_to_square(leds->mapping[i], x, y, size);
        break;
      case Shape2D_RING:
        distance = fabs(distance_to_circle(leds->mapping[i], x, y, size));
        break;
      case Shape2D_CIRCLE:
        distance = distance_to_circle(leds->mapping[i], x, y, size);
        break;
      }

      if (distance < 0)
      {
        // full brightness
        leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, render_brightness * 255);
      }
      else if (distance < fade_distance)
      {
        // fade
        brightness = Commands::ease_in_out_cubic(render_brightness * (1 - distance / fade_distance));
        leds->fade_led(leds->mapping[i][0], leds->mapping[i][1], color, brightness * 255);
      }
    }
  }
}