#include <Commands.h>

float sign(float x1, float y1, float x2, float y2, float x3, float y3)
{
  return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
}

bool point_in_triange(float xp, float yp, float x1, float y1, float x2, float y2, float x3, float y3)
{

  float d1, d2, d3;
  bool has_neg, has_pos;

  d1 = sign(xp, yp, x1, y1, x2, y2);
  d2 = sign(xp, yp, x2, y2, x3, y3);
  d3 = sign(xp, yp, x3, y3, x1, y1);

  has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
  has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(has_neg && has_pos);
}

void Commands::mapped_triangle(LedStructure *leds, CommandParams cmd)
{
  MappedTriangle mapped_triangle = cmd.type_params.mapped_triangle;

  float x1 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_triangle.x1) / 255.0 + MAPPING_X_MIN;
  float y1 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_triangle.y1) / 255.0 + MAPPING_Y_MIN;
  float x2 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_triangle.x2) / 255.0 + MAPPING_X_MIN;
  float y2 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_triangle.y2) / 255.0 + MAPPING_Y_MIN;
  float x3 = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_triangle.x3) / 255.0 + MAPPING_X_MIN;
  float y3 = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_triangle.y3) / 255.0 + MAPPING_Y_MIN;
  float brightness = float(cmd.brightness) / 255.0;
  CRGB color = color_from_palette(cmd, mapped_triangle.color);

  for (int i = 0; i < MAPPING_SIZE; i++)
  {

    if (point_in_triange(leds->mapping_x(i), leds->mapping_y(i), x1, y1, x2, y2, x3, y3))
    {
      // full brightness
      leds->fade_led(leds->mapping_sprip_index(i), leds->mapping_led(i), color, brightness * 255);
    }
  }
}