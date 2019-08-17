#include <Dode.h>
#include <Commands.h>

CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

Dode::Dode(void)
{
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_0>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_1>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_2>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_3>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_4>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_5>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_6>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_7>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
}

void Dode::init()
{
  // command_buffer[0].type = COMMAND_SINGLE_COLOR;
  // command_buffer[0].data[0] = HUE_PURPLE;
  // command_buffer[0].data[1] = 10;

  // command_buffer[1].type = COMMAND_KALEIDOSCOPE;
  // command_buffer[1].data[0] = HUE_PINK;
  // command_buffer[1].data[1] = 255;
  // command_buffer[1].data[2] = 20;

  command_buffer[1].type = COMMAND_RANDOM_WALK;
  command_buffer[1].data[0] = HUE_RED;
  command_buffer[1].data[1] = 255;
  command_buffer[1].data[2] = 1;
  command_buffer[1].data[3] = 100;
  command_buffer[1].data[4] = 1;
}

void Dode::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index < EDGE_COUNT)
  {
    edge_leds[edges[strip_index][0]][edges[strip_index][1] + led] = color;
  }
  else
  {
    for (int i = 0; i < HW_STRIP_COUNT; i++)
    {
      for (int j = 0; j < HW_STRIP_PIXEL_COUNT; j++)
      {
        edge_leds[i][j] = color;
      }
    }
  }
}

CRGB get_led(uint8_t strip_index, int led)
{
  if (strip_index < EDGE_COUNT)
  {
    return edge_leds[edges[strip_index][0]][edges[strip_index][1] + led];
  }
  else
  {
    return edge_leds[edges[0][0]][edges[0][1] + led];
  }
}

void Dode::fade_led(uint8_t strip_index, int led, CRGB target, float amount)
{
  if (led >= 0 && led < EDGE_PIXEL_COUNT)
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(strip_index, led, faded);
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

void Dode::all_white()
{
  for (int i = 0; i < EDGE_PIXEL_COUNT; i++)
  {
    set_led(EDGE_COUNT, i, CRGB(255, 255, 255));
  }
}

uint8_t Dode::random_strip(uint8_t strip_index)
{
  if (strip_index < EDGE_COUNT)
  {
    return strip_index;
  }
  else
  {
    return random8(0, EDGE_COUNT);
  }
}

uint16_t Dode::strip_length(uint8_t strip_index)
{
  return EDGE_PIXEL_COUNT;
}