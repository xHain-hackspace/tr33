#include <Wand.h>
#include <Wand_config.h>

Wand::Wand()
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(LedStructure::leds[0], STRIP_PIXEL_COUNT);
  FastLED.setCorrection(TypicalLEDStrip);
}
