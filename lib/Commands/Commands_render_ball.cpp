#include <Tr33.h>
#include <Commands.h>

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

void Commands::render_nyan(Leds *leds, uint8_t strip_index, float center, float width, CRGB color, float ball_brightness, bool reverse)
{
  if (width != 0)
  {
    float start = center - 0.5;
    float end = start - width;

    if (reverse)
    {
      start = center + 0.5;
      end = start + width;
    }

    float max_brightness = ball_brightness * 0.8;
    float slope = max_brightness / (start - end);

    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));

    for (int i = first_led; i < last_led; i++)
    {
      float brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (brightness > 0)
      {
        CRGB nyan_color = ColorFromPalette(currentPalette, i * 3);
        leds->fade_led(strip_index, i, nyan_color, brightness);
      }
    }
  }

  Commands::render_ball(leds, strip_index, center, 5, CRGB(255, 255, 255), ball_brightness);
}

void Commands::render_comet(Leds *leds, uint8_t strip_index, float center, float width, CRGB color, float ball_brightness, bool reverse)
{
  if (width != 0)
  {
    float start = center - 0.5;
    float end = start - width;

    if (reverse)
    {
      start = center + 0.5;
      end = start + width;
    }

    float max_brightness = ball_brightness * 0.8;
    float slope = max_brightness / (start - end);

    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));

    for (int i = first_led; i < last_led; i++)
    {
      float brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (brightness > 0)
      {
        leds->fade_led(strip_index, i, color, brightness);
      }
    }
  }

  Commands::render_ball(leds, strip_index, center, 5, color, ball_brightness);
}

void Commands::render_power_ball(Leds *leds, uint8_t strip_index, float center, float width, CRGB color, float ball_brightness, uint8_t power)
{
  int start_led = max(0, int(ceilf(center - width / 2.0)));
  int end_led = min((int)leds->strip_length(strip_index), int(floorf(center + width / 2.0)));
  float brightness = 0.0;

  for (int i = start_led; i <= end_led; i++)
  {
    brightness = 1.0 - fabs(float(i) - center) / (width / 2.0);
    brightness = Commands::ease_in_out_power(brightness * ball_brightness, power);
    leds->fade_led(strip_index, i, color, brightness);
  }
}

