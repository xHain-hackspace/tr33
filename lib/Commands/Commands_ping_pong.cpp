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

float Commands::ping_pong_sine(uint8_t period_100ms, uint8_t offset_100ms)
{
  return (sinf(float(millis() - offset_100ms * 100) * 2.0 * PI / float(period_100ms * 100)) + 1.0) / 2.0;
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