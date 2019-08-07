#include <Commands.h>

float Commands::ping_pong_linear(uint8_t period_100ms)
{
  int period = period_100ms * 100;
  int rem = millis() % period;

  if (rem <= period / 2)
  {
    return float(rem) / (float(period) / 2.0);
  }
  else
  {
    return 1.0 - (float(rem) - float(period) / 2.0) / (float(period) / 2.0);
  }
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
