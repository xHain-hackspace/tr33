#include <Tr33.h>
#include <Commands.h>

void Commands::render(Leds *leds, uint8_t render_type, uint8_t strip_index, float position, float width, CRGB color, float brightness)
{
  switch (render_type)
  {
  case RENDER_BALL:
    render_ball(leds, strip_index, position, width, color, brightness);
    break;
  case RENDER_COMET:
    render_comet(leds, strip_index, position, width, color, brightness, false, false);
    break;
  case RENDER_COMET_BOUNCE:
    render_comet(leds, strip_index, position, width, color, brightness, false, true);
    break;
  case RENDER_NYAN:
    render_comet(leds, strip_index, position, width, color, brightness, true, false);
    break;
  case RENDER_NYAN_BOUNCE:
    render_comet(leds, strip_index, position, width, color, brightness, true, true);
    break;
  case RENDER_FILL:
    render_fill(leds, strip_index, position, color, brightness);
    break;
  }
}

void Commands::render_ball(Leds *leds, uint8_t strip_index, float position, float width, CRGB color, float ball_brightness)
{
  position = fabs(position);
  int start_led = max(0, int(ceilf(position - width / 2.0)));
  int end_led = min((int)leds->strip_length(strip_index), int(floorf(position + width / 2.0)));
  float brightness = 0.0;

  for (int i = start_led; i <= end_led; i++)
  {
    brightness = 1.0 - fabs(float(i) - position) / (width / 2.0);
    brightness = Commands::ease_in_out_cubic(brightness * ball_brightness);
    leds->fade_led(strip_index, i, color, brightness);
  }
}

// void Commands::render_nyan(Leds *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool bounce)
// {
//   bool reverse = false;
//   if (position < 0)
//   {
//     position = position * -1;
//     reverse = true;
//   }

//   if (width != 0)
//   {
//     float start = position - 0.5;
//     float end = start - width;

//     if (reverse)
//     {
//       start = position + 0.5;
//       end = start + width;
//     }

//     float max_brightness = brightness * 0.8;
//     float slope = max_brightness / (start - end);

//     int first_led = min(floorf(start), floorf(end));
//     int last_led = max(ceilf(start), ceilf(end));

//     for (int i = first_led; i < last_led; i++)
//     {
//       float brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
//       if (brightness > 0)
//       {
//         CRGB nyan_color = ColorFromPalette(currentPalette, i * 3);
//         leds->fade_led(strip_index, i, nyan_color, brightness);
//       }
//     }
//   }

//   Commands::render_ball(leds, strip_index, position, 5, CRGB(255, 255, 255), brightness);
// }

void Commands::render_comet(Leds *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool nyan, bool bounce)
{
  bool reverse = false;
  if (position < 0)
  {
    position = fabs(position);
    reverse = true;
  }

  if (width != 0)
  {
    float start = position - 0.5;
    float end = start - width;

    if (reverse)
    {
      start = position + 0.5;
      end = start + width;
    }

    float max_brightness = brightness * 0.8;
    float slope = max_brightness / (start - end);

    uint16_t max_led = leds->strip_length(strip_index) - 1;
    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));
    int led = 0;
    for (int i = first_led; i < last_led; i++)
    {

      float tail_brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (tail_brightness > 0)
      {
        led = i;
        if (bounce && i < 0)
        {
          led = i * -1;
        }
        else if (bounce && i > max_led)
        {
          led = max_led - (i - max_led);
        }

        if (nyan)
        {
          color = ColorFromPalette(currentPalette, led * 3);
        }

        leds->fade_led(strip_index, led, color, tail_brightness);
      }
    }
  }
  if (nyan)
  {
    color = COLOR_WHITE;
  }
  Commands::render_ball(leds, strip_index, position, 5, color, brightness);
}

void Commands::render_fill(Leds *leds, uint8_t strip_index, float position, CRGB color, float brightness)
{
  int first_led = 0;
  int last_led = 0;
  if (position > 0)
  {
    last_led = floorf(position);
  }
  else
  {
    first_led = ceilf(fabs(position));
    last_led = leds->strip_length(strip_index);
  }

  for (int i = first_led; i < last_led; i++)
  {
    leds->fade_led(strip_index, i, color, brightness);
  }
}
