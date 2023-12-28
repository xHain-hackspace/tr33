#include <Tetraeder.h>
#include <Tetraeder_config.h>

Tetraeder::Tetraeder()
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

std::array<int, 2> map(uint8_t strip_index, int led)
{
  switch (strip_index)
  {
  case 0:
    return {4, led + EDGE_LENGTH};
  case 1:
    return {3, led + EDGE_LENGTH};
  case 2:
    return {10, led + EDGE_LENGTH};

  case 3:
    return {3, led};
  case 4:
    return {10, led};
  case 5:
    return {4, led};

  case 6:
    return {7, led + EDGE_LENGTH};
  case 7:
    return {11, led + EDGE_LENGTH};
  case 8:
    return {6, led + EDGE_LENGTH};
  case 9:
    return {9, led + EDGE_LENGTH};

  case 10:
    return {5, led + EDGE_LENGTH};
  case 11:
    return {1, led + EDGE_LENGTH};
  case 12:
    return {0, led + EDGE_LENGTH};

  case 13:
    return {2, led + EDGE_LENGTH};
  case 14:
    return {8, led + EDGE_LENGTH};

  case 15:
    return {11, led};
  case 16:
    return {9, led};

  case 17:
    return {5, led};
  case 18:
    return {0, led};

  case 19:
    return {2, led};
  case 20:
    return {7, led};

  case 21:
    return {8, led};
  case 22:
    return {6, led};
  case 23:
    return {1, led};
  }
}

int corners[CORNER_COUNT][6] = {
    {MINUS_NULL, -1, -2, NO_EDGE, NO_EDGE, NO_EDGE},
    {0, 3, -6, -7, -14, -5},
    {1, -3, 4, -8, -9, -10},
    {2, -4, 5, -12, -11, -13},
    {6, 15, -20, NO_EDGE, NO_EDGE, NO_EDGE},
    {15, 21, -22, 16, 7, 8},
    {-16, 9, 17, NO_EDGE, NO_EDGE, NO_EDGE},
    {22, 10, 11, -17, -23, 18},
    {12, -18, 19, NO_EDGE, NO_EDGE, NO_EDGE},
    {-19, 23, -21, 13, 14, 20}};

CRGB Tetraeder::get_led(uint8_t strip_index, int led)
{
  std::array<int, 2> mapped = map(strip_index, led);
  return leds[mapped[0]][mapped[1]];
}

void Tetraeder::set_led(uint8_t strip_index, int led, CRGB color)
{
  std::array<int, 2> mapped = map(strip_index, led);
  leds[mapped[0]][mapped[1]] = color;
}

void Tetraeder::fade_led(CommandParams cmd, int led, CRGB target)
{
  fade_led(cmd.strip_index, led, target, cmd.brightness);
}

void Tetraeder::fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount);
    set_led(strip_index, led, faded);
  }
}

uint16_t Tetraeder::strip_length(uint8_t strip_index)
{
  return EDGE_LENGTH;
}

uint16_t Tetraeder::pixel_count(uint8_t strip_index)
{
  return EDGE_LENGTH * EDGE_COUNT;
}

uint8_t Tetraeder::random_strip(uint8_t strip_index)
{
  return random8(0, EDGE_COUNT);
}

uint8_t Tetraeder::get_max_strip_index()
{
  return EDGE_COUNT;
}
