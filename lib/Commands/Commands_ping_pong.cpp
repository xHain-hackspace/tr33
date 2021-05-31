#include <Commands.h>

void Commands::ping_pong(LedStructure *leds, CommandParams cmd)
{
  PingPong ping_pong = cmd.type_params.ping_pong;

  float brightness = float(cmd.brightness) / 255;
  float max_height = float(ping_pong.max_height) / 255;

  CRGB color = color_from_palette(cmd, ping_pong.color);

  float position = ping_pong_fraction(ping_pong.movement, ping_pong.period_ms, ping_pong.offset_ms) * float(leds->strip_length(cmd.strip_index) - 1) * max_height;

  render(leds, ping_pong.shape, cmd.strip_index, fabs(position), ping_pong.width, color, brightness, position > 0);
}

float Commands::ping_pong_fraction(MovementType movement, uint8_t period_100ms, uint8_t offset_100ms)
{
  float fraction = 0.0;
  switch (movement)
  {
  case MovementType_LINEAR:
    fraction = ping_pong_linear(period_100ms, offset_100ms);
    break;
  case MovementType_SINE:
    fraction = ping_pong_sine(period_100ms, offset_100ms);
    break;
  case MovementType_SAWTOOTH:
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

float Commands::ping_pong_sawtooth(uint8_t period_100ms, uint8_t offset_100ms)
{
  int period_millis = period_100ms * 100;
  int rem = (millis() + offset_100ms * 100) % period_millis;
  return float(rem) / float(period_millis);
}