#include <Dode.h>
#include <Commands.h>

void Dode::kaleidoscope(char *data)
{
  uint8_t color_index = data[0];
  float brightness = data[1];
  uint8_t period = data[2];
  uint8_t offset = data[3];

  float width = fabs(Commands::ping_pong_sine(period, offset)) * EDGE_MAX_LENGTH * 1.2;
  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    Commands::render_ball(this, i, strip_length(i) / 2, width, color, brightness / 255);
  }
}
