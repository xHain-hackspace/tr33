#include <Trommel.h>
#include <Trommel_config.h>

Trommel::Trommel()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(leds[0], STRIP_PIXEL_COUNT);
}
