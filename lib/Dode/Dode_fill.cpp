#include <Dode.h>
#include <Commands.h>

void ball(Leds *leds, float rel_position, float width, CRGB color, float brightness)
{
  for (int i = 0; i < EDGE_COUNT; i++)
  {
    Commands::render_ball(leds, i, leds->strip_length(i) * rel_position, width, color, brightness);
  }
}

void fill_bottom(Leds *leds, float rel_position, float width, CRGB color, float brightness)
{
  for (int i = 0; i < EDGE_COUNT; i++)
  {
    float position = leds->strip_length(i) * rel_position;
    Commands::render_ball(leds, i, position, 5, color, brightness);

    for (int j = 0; j < position; j++)
    {
      leds->fade_led(i, j, color, brightness);
    }
  }
}

void fill_top(Leds *leds, float rel_position, float width, CRGB color, float brightness)
{
  for (int i = 0; i < EDGE_COUNT; i++)
  {
    float position = leds->strip_length(i) * rel_position;
    Commands::render_ball(leds, i, position, 5, color, brightness);

    for (int j = ceilf(position); j < leds->strip_length(i); j++)
    {
      leds->fade_led(i, j, color, brightness);
    }
  }
}

void Dode::fill(char *data)
{
  uint8_t type = data[0];
  uint8_t color_index = data[1];
  float brightness = float(data[2]) / 255;
  float rel_position = float(data[3]) / 255.0;
  float width = data[4];

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  switch (type)
  {
  case FILL_BALL:
    ball(this, rel_position, width, color, brightness);
    break;
  case FILL_BOTTOM:
    fill_bottom(this, rel_position, width, color, brightness);
    break;
  case FILL_TOP:
    fill_top(this, rel_position, width, color, brightness);
    break;
  }
}
