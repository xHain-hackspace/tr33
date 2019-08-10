#include <Tr33.h>
#include <Commands.h>

void Tr33::render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom)
{
  switch (ball_type)
  {
  case BALL_TYPE_SQUARE:
    render_square_ball(strip_index, fabs(center), width, color, ball_brightness);
    break;
  case BALL_TYPE_SINE:
    Commands::render_ball(this, strip_index, fabs(center), width, color, ball_brightness);
    break;
  case BALL_TYPE_COMET:
    render_comet(strip_index, center, width * 10.0, color, ball_brightness, bounce_top, bounce_bottom);
    break;
  case BALL_TYPE_NYAN:
    render_nyan(strip_index, center, width * 3.0, color, ball_brightness, bounce_top, bounce_bottom);
    break;
  case BALL_TYPE_FILL_TOP:
    render_fill_top(strip_index, center, color, ball_brightness);
    break;
  case BALL_TYPE_FILL_BOTTOM:
    render_fill_bottom(strip_index, center, color, ball_brightness);
    break;
  }
}

void Tr33::render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness)
{
  int length = strip_length(strip_index);
  int start_led = max(0, int(ceilf(center - width / 2.0)));
  int end_led = min(length, int(floorf(center + width / 2.0)));

  for (int i = start_led; i <= end_led; i++)
  {
    fade_led(strip_index, i, color, ball_brightness);
  }
}

void Tr33::render_comet(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom)
{
  Commands::render_ball(this, strip_index, fabs(center), 3, color, ball_brightness);

  if (width != 0)
  {
    float start;
    if (center > 0)
    {
      start = center - 0.5;
    }
    else
    {
      start = fabs(center) + 0.5;
      width = width * -1;
    }

    uint16_t length = strip_length(strip_index);
    float end = start - width;
    float max_brightness = ball_brightness * 0.8;

    float slope = max_brightness / (start - end);

    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));

    for (int i = first_led; i < last_led; i++)
    {
      float brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (brightness > 0)
      {
        int led_index = i;
        if (bounce_bottom && led_index < 0)
        {
          led_index = -1 * led_index;
        }
        if (bounce_top && led_index >= length)
        {
          led_index = 2 * (length)-led_index;
        }
        fade_led(strip_index, led_index, color, brightness);
      }
    }
  }
}

void Tr33::render_nyan(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom)
{

  if (width != 0)
  {
    float start;
    if (center > 0)
    {
      start = center - 0.5;
    }
    else
    {
      start = fabs(center) + 0.5;
      width = width * -1;
    }

    uint16_t length = strip_length(strip_index);
    float end = start - width;
    float max_brightness = ball_brightness * 0.8;

    float slope = max_brightness / (start - end);

    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));

    for (int i = first_led; i < last_led; i++)
    {
      float brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (brightness > 0)
      {
        int led_index = i;
        if (bounce_bottom && led_index < 0)
        {
          led_index = -1 * led_index;
        }
        if (bounce_top && led_index >= length)
        {
          led_index = 2 * (length)-led_index;
        }
        CRGB nyan_color = ColorFromPalette(currentPalette, i * 10);
        fade_led(strip_index, led_index, nyan_color, brightness);
      }
    }
  }

  Commands::render_ball(this, strip_index, fabs(center), 5, CRGB(255, 255, 255), ball_brightness);
}

void Tr33::render_fill_top(uint8_t strip_index, float center, CRGB color, float ball_brightness)
{
  Commands::render_ball(this, strip_index, center, 5, color, ball_brightness);

  for (int i = center; i < strip_length(strip_index); i++)
  {
    fade_led(strip_index, i, color, ball_brightness);
  }
}

void Tr33::render_fill_bottom(uint8_t strip_index, float center, CRGB color, float ball_brightness)
{
  Commands::render_ball(this, strip_index, center, 5, color, ball_brightness);

  for (int i = 0; i < center; i++)
  {
    fade_led(strip_index, i, color, ball_brightness);
  }
}