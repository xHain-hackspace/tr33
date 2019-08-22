#include <Dode.h>
#include <Commands.h>

void Dode::ping_pong(char *data)
{
  uint8_t type = data[0];
  uint8_t color_index = data[1];
  uint8_t brightness = data[2];
  uint8_t width = data[3];
  uint8_t period = data[4];
  uint8_t offset = data[5];

  float position;

  switch (type)
  {
  case PING_PONG_LINEAR:
    position = fabs(Commands::ping_pong_linear(period, offset));
    break;
  case PING_PONG_SINE:
    position = fabs(Commands::ping_pong_sine(period, offset));
    break;
  case PING_PONG_COSINE:
    position = fabs(Commands::ping_pong_cosine(period, offset));
    break;
  case PING_PONG_SAWTOOTH:
    position = fabs(Commands::ping_pong_sawtooth(period, offset));
    break;
  }

  CRGB color = ColorFromPalette(currentPalette, color_index, brightness);

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    Commands::render_ball(this, i, position * strip_length(i), width, color, 1);
  }
}
