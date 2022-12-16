#include <Wolke.h>
#include <Wolke_config.h>

Wolke::Wolke()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(leds[0], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(leds[1], STRIP_PIXEL_COUNT);
}

// These need to be adjusted once both strips are actually used
void Wolke::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (led < STRIP_PIXEL_COUNT)
  {
    if (strip_index == STRIP_INDEX_ALL || strip_index == WOLKEN_INDEX)
    {
      leds[0][led] = color;
      leds[1][led] = color;
    }
  }
}

CRGB Wolke::get_led(uint8_t strip_index, int led)
{
  if (led < STRIP_PIXEL_COUNT)
  {
    if (strip_index == STRIP_INDEX_ALL || strip_index == WOLKEN_INDEX)
    {
      return leds[0][led];
    }
    else
    {
      return CRGB(0, 0, 0);
    }
  }
}

uint16_t Wolke::pixel_count(uint8_t strip_index)
{
  if (strip_index == STRIP_INDEX_ALL)
  {
    return STRIP_PIXEL_COUNT * WOLKEN_COUNT;
  }
  else
  {
    return STRIP_PIXEL_COUNT;
  }
}

uint8_t Wolke::random_strip(uint8_t strip_index)
{
  if (strip_index == STRIP_INDEX_ALL)
  {
    return random8(0, WOLKEN_COUNT) + 1;
  }
  else
  {
    return strip_index;
  }
}

String Wolke::get_name()
{
  return "Wolke " + String(WOLKEN_INDEX);
}

uint8_t Wolke::get_max_strip_index()
{
  return WOLKEN_COUNT;
}