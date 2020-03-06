#include <Dode.h>
#include <Commands.h>

void Dode::ping_pong(char *data)
{
  uint8_t ping_pong_type = data[0];
  uint8_t color_index = data[1];
  float brightness = float(data[2]) / 255;
  uint8_t width = data[3];
  uint8_t period = data[4];
  uint8_t offset = data[5];
  float max_height = float(data[6]) / 255;
  uint8_t render_type = data[7];

  float position = 0.0;

  switch (ping_pong_type)
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

  position = position * max_height;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  Commands::render(this, render_type, EDGE_COUNT, position, width, color, brightness);
}
