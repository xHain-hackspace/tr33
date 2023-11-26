#include <Commands.h>

void Commands::render(LedStructure *leds, const CommandParams &cmd)
{
  Render render_p = cmd.type_params.render;

  float brightness = float(cmd.brightness) / 255;
  float position = float(render_p.position) / 65535.0 * float(leds->strip_length(cmd.strip_index));

  CRGB color = color_from_palette(cmd, render_p.color);

  bool direction = false;
  bool wrap = false;
  bool bounce = false;

  render(leds, render_p.shape, cmd.strip_index, position, render_p.width, color, brightness, direction, bounce, wrap);
}

// bounce: reflect comet at the end of the strip, wrap: show out of bound pixels on the other side of the strip
void Commands::render(LedStructure *leds, Shape1D shape, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool bounce, bool wrap)
{
  switch (shape)
  {
  case Shape1D_BALL:
    render_ball(leds, strip_index, position, width, color, brightness, wrap);
    break;
  case Shape1D_COMET:
    render_comet(leds, strip_index, position, width, color, brightness, direction, false, bounce, wrap);
    break;
  case Shape1D_NYAN:
    render_comet(leds, strip_index, position, width, color, brightness, direction, true, bounce, wrap);
    break;
  case Shape1D_FILLED:
    render_fill(leds, strip_index, position, color, brightness, direction);
    break;
  }
}

void Commands::render_ball(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float ball_brightness, bool wrap)
{
  int first_led = int(ceilf(position - width / 2.0));
  int last_led = int(floorf(position + width / 2.0));
  int led_count = leds->strip_length(strip_index);

  if (!wrap)
  {
    first_led = max(0, first_led);
    last_led = min(led_count - 1, last_led);
  }

  float brightness = 0.0;
  int target_led = 0;

  for (int i = first_led; i <= last_led; i++)
  {
    target_led = i;
    // wrap to other side of the strip if out of bounds
    if (i < 0)
    {
      target_led = led_count + i;
    }
    else if (i >= led_count)
    {
      target_led = i - led_count;
    }

    brightness = 1.0 - fabs(float(i) - position) / (width / 2.0);
    brightness = Commands::ease_in_out_cubic(brightness * ball_brightness);
    leds->fade_led(strip_index, target_led, color, brightness * 255);
  }
}

void Commands::render_comet(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool nyan, bool bounce, bool wrap)
{
  if (width != 0)
  {
    float start;
    float end;

    if (direction)
    {
      start = position - 0.5;
      end = start - width;
    }
    else
    {
      start = position + 0.5;
      end = start + width;
    }

    float max_brightness = brightness * 0.8;
    float slope = max_brightness / (start - end);

    uint16_t led_count = leds->strip_length(strip_index);
    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));
    int target_led = 0;

    for (int i = first_led; i < last_led; i++)
    {
      float tail_brightness = Commands::ease_in_out_cubic(slope * (float(i) - end));
      if (tail_brightness > 0)
      {
        target_led = i;
        if (i < 0)
        {
          if (bounce)
          {
            target_led = i * -1;
          }
          if (wrap)
          {
            target_led = led_count + i;
          }
        }
        else if (i >= led_count)
        {
          if (bounce)
          {
            target_led = led_count - (i - led_count) - 1;
          }
          if (wrap)
          {
            target_led = i - led_count;
          }
        }

        if (nyan)
        {
          color = ColorFromPalette(RainbowColors_p, target_led * 3, 255);
        }

        leds->fade_led(strip_index, target_led, color, tail_brightness * 255);
      }
    }
  }
  if (nyan)
  {
    color = COLOR_WHITE;
  }
  Commands::render_ball(leds, strip_index, position, 5, color, brightness, wrap);
}

void Commands::render_fill(LedStructure *leds, uint8_t strip_index, float position, CRGB color, float brightness, bool direction)
{
  int first_led = 0;
  int last_led = 0;
  if (direction)
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
    leds->fade_led(strip_index, i, color, brightness * 255);
  }
}
