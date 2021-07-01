#include <Discotestrig.h>
#include <Discotestrig_config.h>

Discotestrig::Discotestrig()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LedStructure::leds[0], STRIP_PIXEL_COUNT);
}
