#include <Commands.h>

void Commands::ping_pong(LedStructure *leds, uint8_t *data)
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

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);
  float position = ping_pong_fraction(ping_pong_type, period, offset) * float(leds->strip_length(strip_index) - 1) * max_height;

  render(leds, render_type, strip_index, fabs(position), width, color, brightness, position > 0);
}

float Commands::ping_pong_fraction(uint8_t ping_pong_type, uint8_t period_100ms, uint8_t offset_100ms)
{
  float fraction = 0.0;
  switch (ping_pong_type)
  {
  case PING_PONG_LINEAR:
    fraction = ping_pong_linear(period_100ms, offset_100ms);
    break;
  case PING_PONG_SINE:
  case PING_PONG_NONE:
    fraction = ping_pong_sine(period_100ms, offset_100ms);
    break;
  case PING_PONG_SAWTOOTH:
    fraction = ping_pong_sawtooth(period_100ms, offset_100ms);
    break;
  }

  return fraction;
}

float Commands::ping_pong_linear(uint8_t period_100ms, uint8_t offset_100ms)
{
  int period_millis = period_100ms * 100;
  int rem = (millis() + offset_100ms * 100) % period_millis;

  if (rem <= period_millis / 2)
  {
    return float(rem) / (float(period_millis) / 2.0);
  }
  else
  {
    return (1.0 - (float(rem) - float(period_millis) / 2.0) / (float(period_millis) / 2.0)) * -1.0;
  }
}

float Commands::ping_pong_sine(uint8_t period_100ms, uint8_t offset_100ms)
{
  int rem = (millis() + offset_100ms * 100) % (period_100ms * 100);

  float res = (sinf(float(rem) * 2.0 * PI / float(period_100ms * 100) - PI / 2) + 1.0) / 2.0;

  if (rem <= period_100ms * 100 / 2)
  {
    return res;
  }
  else
  {
    return res * -1;
  }
}

float Commands::ping_pong_cosine(uint8_t period_100ms, uint8_t offset_100ms)
{
  return (cosf(float(millis() - offset_100ms * 100) * 2.0 * PI / float(period_100ms * 100)) + 1.0) / 2.0;
}

float Commands::ping_pong_sawtooth(uint8_t period_100ms, uint8_t offset_100ms)
{
  int period_millis = period_100ms * 100;
  int rem = (millis() + offset_100ms * 100) % period_millis;
  return float(rem) / float(period_millis);
}