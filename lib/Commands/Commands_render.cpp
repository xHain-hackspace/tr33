#include <Commands.h>

void Commands::render(LedStructure *leds, CommandParams cmd)
{
  Render render_p = cmd.type_params.render;

  float brightness = float(cmd.brightness) / 255;
  uint16_t position_int = render_p.position;
  float position = float(position_int) / 65535.0 * float(leds->strip_length(cmd.strip_index));

  CRGB color = color_from_palette(cmd, render_p.color);

  render(leds, render_p.shape, cmd.strip_index, position, render_p.width, color, brightness, false);
}

void Commands::render(LedStructure *leds, Shape1D shape, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction)
{
  switch (shape)
  {
  case Shape1D_BALL:
    render_ball(leds, strip_index, position, width, color, brightness);
    break;
  case Shape1D_COMET:
    render_comet(leds, strip_index, position, width, color, brightness, direction, false, false);
    break;
  case Shape1D_COMET_BOUNCE:
    render_comet(leds, strip_index, position, width, color, brightness, direction, false, true);
    break;
  case Shape1D_NYAN:
    render_comet(leds, strip_index, position, width, color, brightness, direction, true, false);
    break;
  case Shape1D_NYAN_BOUNCE:
    render_comet(leds, strip_index, position, width, color, brightness, direction, true, true);
    break;
  case Shape1D_SOLID:
    render_fill(leds, strip_index, position, color, brightness, direction);
    break;
  }
}

void Commands::render_ball(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float ball_brightness)
{
  position = position;
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

void Commands::render_comet(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool nyan, bool bounce)
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
          color = ColorFromPalette(RainbowColors_p, led * 3, 255);
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
    leds->fade_led(strip_index, i, color, brightness);
  }
}
