#include <Minichrome.h>
#include <Minichrome_config.h>

Minichrome::Minichrome()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(leds[0], STRIP_PIXEL_COUNT);
}
