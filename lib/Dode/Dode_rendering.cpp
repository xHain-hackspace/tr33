#include <Dode.h>
#include <Commands.h>

void Dode::set_led(uint8_t edge_index, uint8_t led, CRGB color)
{
  leds[edges[edge_index][0]][edges[edge_index][1] + led] = color;
}

CRGB Dode::get_led(uint8_t edge_index, uint8_t led)
{
  return leds[edges[edge_index][0]][edges[edge_index][1] + led];
}

void Dode::fade_led(uint8_t edge_index, int led, CRGB target, float amount)
{
  if (led >= 0 && led < EDGE_PIXEL_COUNT)
  {
    CRGB current = get_led(edge_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(edge_index, led, faded);
  }
}

void Dode::set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color)
{
  if (led_index <= EDGE_PIXEL_COUNT / 2 - 1)
  {
    set_led(edge_index, EDGE_PIXEL_COUNT / 2 - 1 - led_index, color);
    set_led(edge_index, EDGE_PIXEL_COUNT / 2 + led_index, color);
  }
}

void Dode::render_ball(int8_t edge, float center, float width, CRGB color, float ball_brightness)
{
  if (edge < 0)
  {
    center = EDGE_PIXEL_COUNT - center;
  }

  int start_led = max(0, int(ceilf(center - width / 2.0)));
  int end_led = min(EDGE_PIXEL_COUNT, int(floorf(center + width / 2.0)));
  float brightness = 0.0;

  for (int i = start_led; i <= end_led; i++)
  {
    brightness = 1.0 - fabs(float(i) - center) / (width / 2.0);
    brightness = Commands::ease_in_out_cubic(brightness * ball_brightness);
    fade_led(abs8(edge) - 1, i, color, brightness);
  }
}

void Dode::all_off()
{
  for (int i = 0; i < HW_STRIP_COUNT; i++)
  {
    for (int j = 0; j < HW_STRIP_PIXEL_COUNT; j++)
    {
      leds[i][j] = CRGB(0, 0, 0);
    }
  }
}