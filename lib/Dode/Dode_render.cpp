#include <Dode.h>
#include <Commands.h>
void Dode::render(uint8_t *data)
{
  uint8_t type = data[0];
  uint8_t color_index = data[1];
  float brightness = float(data[2]) / 255;
  float position = float(data[3]) / float(255) * float(EDGE_MAX_LENGTH);
  float width = data[4];

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  Commands::render(this, type, EDGE_COUNT, position, width, color, brightness, false);
}
