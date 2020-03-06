#include <Keller.h>
#include <Commands.h>

void Keller::render(char *data)
{
  uint8_t render_type = data[0];
  uint8_t strip_index = data[1];
  uint8_t color_index = data[2];
  float brightness = float(data[3]) / 255;
  float position = float(data[4]) / (255) * STRIP_PIXEL_COUNT;
  float width = data[5];

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  Commands::render(this, render_type, strip_index, position, width, color, brightness);
}
