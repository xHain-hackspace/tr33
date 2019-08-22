#include <Commands.h>

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

float Commands::ping_pong_sin(uint8_t period_100ms, uint8_t offset_100ms)
{
  return (sinf(float(millis() - offset_100ms * 100) * 2.0 * PI / float(period_100ms * 100)) + 1.0) / 2.0;
}

float Commands::ping_pong_cos(uint8_t period_100ms, uint8_t offset_100ms)
{
  return (cosf(float(millis() - offset_100ms * 100) * 2.0 * PI / float(period_100ms * 100)) + 1.0) / 2.0;
}

float Commands::ease_in_cubic(float t)
{
  return t * t * t;
}

float Commands::ease_out_cubic(float t)
{
  t -= 1;
  return t * t * t + 1;
}

float Commands::ease_in_out_cubic(float t)
{
  t *= 2;
  if (t < 1)
    return 0.5f * t * t * t;
  t -= 2;
  return 0.5f * (t * t * t + 2);
}

uint8_t Commands::random_or_value(uint8_t value, uint8_t min, uint8_t max)
{
  if (value == 0)
  {
    return random8(min, max);
  }
  else
  {
    return value;
  }
}
