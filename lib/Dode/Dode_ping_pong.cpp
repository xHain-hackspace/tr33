#include <Dode.h>
#include <Commands.h>

void Dode::ping_pong(char *data)
{
  uint8_t type = data[0];
  uint8_t color_index = data[1];
  float brightness = float(data[2]) / 255;
  uint8_t width = data[3];
  uint8_t period = data[4];
  uint8_t offset = data[5];
  float max_height = float(data[6]) / 255;
  uint8_t fill_type = data[7];

  float position = 0.0;

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

  position = position * max_height;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  fill(this, position, width, color, brightness, fill_type);
}
