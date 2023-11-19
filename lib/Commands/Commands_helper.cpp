#include <Commands.h>

float Commands::ease_in_quad(float t)
{
  return t * t;
}

float Commands::ease_out_quad(float t)
{
  t -= 1;
  return 1 - t * t;
}

float Commands::ease_in_out_quad(float t)
{
  t *= 2;
  if (t < 1)
    return 0.5f * t * t;
  t -= 2;
  return 0.5f * (t * t + 2);
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
