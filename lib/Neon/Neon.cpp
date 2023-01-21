#include <Neon.h>
#include <Neon_config.h>

Neon::Neon()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LedStructure::leds[0], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(LedStructure::leds[1], STRIP_PIXEL_COUNT);
}
