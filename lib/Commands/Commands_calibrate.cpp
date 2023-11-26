#include <Commands.h>
#include <Modifiers.h>

void Commands::calibrate(LedStructure *leds, const CommandParams &cmd)
{
  Calibrate calibrate = cmd.type_params.calibrate;
  FastLED.setCorrection(UncorrectedColor);
  for (int i = 0; i < 80; i++)
  {
    leds->set_led(cmd.strip_index, i, CRGB(calibrate.red, calibrate.green, calibrate.blue));
  }
}