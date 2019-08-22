#include <Dode.h>
#include <Commands.h>

void Dode::ping_pong(char *data)
{
  uint8_t color_index = data[0];
  uint8_t brightness = data[1];
  uint8_t width = data[2];
  uint8_t period = data[3];
  uint8_t offset = data[4];

  float position = fabs(Commands::ping_pong_sin(period, offset)) * EDGE_MAX_LENGTH / 2;
  CRGB color = ColorFromPalette(currentPalette, color_index, brightness);

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    Commands::render_ball(this, i, position * strip_length(i), width, color, 1);
  }
}
