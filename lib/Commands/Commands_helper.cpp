#include <Commands.h>
#include <cmath>

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

float Commands::ease_in_power(float t, uint8_t p)
{
  return pow(t,p);
}

float Commands::ease_out_power(float t, uint8_t p)
{
  t -= 1;
  return pow(t,p) + 1;
}

float Commands::ease_in_out_power(float t, uint8_t p)
{
  t *= 2;
  if (t < 1)
    return 0.5f * pow(t,p);
  t -= 2;
  return 0.5f * (pow(t,p) + 2);
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
