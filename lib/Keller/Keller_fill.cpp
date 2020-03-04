#include <Keller.h>
#include <Commands.h>

void ball(Leds *leds, uint8_t strip_index, float rel_position, float width, CRGB color, float brightness)
{
 Commands::render_ball(leds, strip_index, float(STRIP_PIXEL_COUNT) * rel_position, width, color, brightness);
}

// void fill_bottom(Leds *leds, float rel_position, float width, CRGB color, float brightness)
// {
//   for (int i = 0; i < EDGE_COUNT; i++)
//   {
//     float position = leds->strip_length(i) * rel_position;
//     Commands::render_ball(leds, i, position, 5, color, brightness);

//     for (int j = 0; j < position; j++)
//     {
//       leds->fade_led(i, j, color, brightness);
//     }
//   }
// }

// void fill_top(Leds *leds, float rel_position, float width, CRGB color, float brightness)
// {
//   for (int i = 0; i < EDGE_COUNT; i++)
//   {
//     float position = leds->strip_length(i) * rel_position;
//     Commands::render_ball(leds, i, position, 5, color, brightness);

//     for (int j = ceilf(position); j < leds->strip_length(i); j++)
//     {
//       leds->fade_led(i, j, color, brightness);
//     }
//   }
// }

void Keller::fill(Leds *leds, uint8_t strip_index, float rel_position, float width, CRGB color, float brightness, uint8_t type)
{
  switch (type)
  {
  case FILL_BALL:
    ball(this, strip_index, rel_position, width, color, brightness);
    break;
  // case FILL_BOTTOM:
  //   fill_bottom(this, rel_position, width, color, brightness);
  //   break;
  // case FILL_TOP:
  //   fill_top(this, rel_position, width, color, brightness);
  //   break;
  }
}

void Keller::fill(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t type = data[1];
  uint8_t color_index = data[2];
  float brightness = float(data[3]) / 255;
  float rel_position = float(data[4]) / 255;
  float width = data[5];

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  fill(this, strip_index, rel_position, width, color, brightness, type);
}
