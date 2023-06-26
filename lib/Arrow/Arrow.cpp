#include <Arrow.h>
#include <Arrow_config.h>

Arrow::Arrow()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(leds[0], STRIP_PIXEL_COUNT);
}
