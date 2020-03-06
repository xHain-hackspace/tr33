#include <Commands.h>
#include <Keller.h>

void Keller::ping_pong(char *data)
{
  uint8_t render_type = data[0];
  uint8_t strip_index = data[1];
  uint8_t color_index = data[2];
  float brightness = float(data[3]) / 255;
  float width = data[4];
  uint8_t ping_pong_type = data[5];
  uint8_t period = data[6];
  uint8_t offset = data[7];
  float max_height = float(data[8]) / 255;

  float position = 0.0;

  switch (ping_pong_type)
  {
  case PING_PONG_LINEAR:
    position = Commands::ping_pong_linear(period, offset) * float(STRIP_PIXEL_COUNT - 1);
    break;
  case PING_PONG_SINE:
  case PING_PONG_COSINE:
    position = Commands::ping_pong_sine(period, offset) * float(STRIP_PIXEL_COUNT - 1);
    break;
  case PING_PONG_SAWTOOTH:
    position = Commands::ping_pong_sawtooth(period, offset) * float(STRIP_PIXEL_COUNT - 1);
    break;
  }

  position = position * max_height;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  Commands::render(this, render_type, strip_index, fabs(position), width, color, brightness, position > 0);
}
