#include <Commands.h>
#include <Leds.h>

void Commands::render_ball(Leds *leds, uint8_t strip_index, float center, float width, CRGB color, float ball_brightness)
{
  int start_led = max(0, int(ceilf(center - width / 2.0)));
  int end_led = min((int)leds->strip_length(strip_index), int(floorf(center + width / 2.0)));
  float brightness = 0.0;

  for (int i = start_led; i <= end_led; i++)
  {
    brightness = 1.0 - fabs(float(i) - center) / (width / 2.0);
    brightness = Commands::ease_in_out_cubic(brightness * ball_brightness);
    leds->fade_led(strip_index, i, color, brightness);
  }
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
