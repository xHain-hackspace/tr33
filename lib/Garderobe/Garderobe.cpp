#include <Garderobe.h>
#include <Garderobe_config.h>

Garderobe::Garderobe()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LedStructure::leds[0], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(LedStructure::leds[1], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(LedStructure::leds[2], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_4>(LedStructure::leds[3], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_5>(LedStructure::leds[4], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_6>(LedStructure::leds[5], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_7>(LedStructure::leds[6], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_8>(LedStructure::leds[7], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_9>(LedStructure::leds[8], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_10>(LedStructure::leds[9], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_11>(LedStructure::leds[10], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_12>(LedStructure::leds[11], STRIP_PIXEL_COUNT);
}

std::array<int, 2> Garderobe::map(uint8_t strip_index, int led)
{
  if (strip_index == 0)
  {
    return std::array<int, 2>{0, led};
  }

  strip_index--;

  // Calculate which physical strip this maps to (2 edges = 1 strip)
  int physical_strip = strip_index / 2;

  // Determine if this is the second edge of the strip (odd indices)
  bool is_second_edge = (strip_index % 2) == 1;

  // For second edge, invert the LED index
  if (is_second_edge)
  {
    led = EDGE_PIXEL_COUNT - 1 - led;
  }

  return std::array<int, 2>{physical_strip, led};
}

CRGB Garderobe::get_led(uint8_t strip_index, int led)
{
  std::array<int, 2> mapped = map(strip_index, led);
  return leds[mapped[0]][mapped[1]];
}

void Garderobe::set_led(uint8_t strip_index, int led, CRGB color)
{
  // Only proceed if LED index is within bounds
  if (led >= 0 && led < EDGE_PIXEL_COUNT)
  {
    if (strip_index == 0)
    {
      for (uint8_t edge = 1; edge <= EDGE_COUNT; edge++)
      {
        set_led(edge, led, color);
      }
    }
    else
    {
      std::array<int, 2> mapped = map(strip_index, led);
      leds[mapped[0]][mapped[1]] = color;
    }
  }
}

void Garderobe::fade_led(CommandParams cmd, int led, CRGB target)
{
  fade_led(cmd.strip_index, led, target, cmd.brightness);
}

void Garderobe::fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount);
    set_led(strip_index, led, faded);
  }
}

uint16_t Garderobe::strip_length(uint8_t strip_index)
{
  return EDGE_PIXEL_COUNT;
}

uint16_t Garderobe::pixel_count(uint8_t strip_index)
{
  if (strip_index == 0)
  {
    return EDGE_PIXEL_COUNT * EDGE_COUNT;
  }
  else
  {
    return EDGE_PIXEL_COUNT;
  }
}

uint8_t Garderobe::random_strip(uint8_t strip_index)
{
  return random8(1, EDGE_COUNT + 1);
}

uint8_t Garderobe::get_max_strip_index()
{
  return EDGE_COUNT;
}
