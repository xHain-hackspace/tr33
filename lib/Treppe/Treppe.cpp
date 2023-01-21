#include <Treppe.h>
#include <Treppe_config.h>

Treppe::Treppe()
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
