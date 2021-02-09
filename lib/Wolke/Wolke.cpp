#include <Wolke.h>
#include <Wolke_config.h>

Wolke::Wolke()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(leds[0], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(leds[1], STRIP_PIXEL_COUNT);
}
