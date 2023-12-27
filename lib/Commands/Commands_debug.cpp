#include <Commands.h>
#include <Modifiers.h>
#include <ColorTools.h>

void Commands::debug(LedStructure *leds, const CommandParams &cmd)
{
  Debug debug = cmd.type_params.debug;

  CRGB color = CRGB(128, 128, 0);
  ColorTools::apply_gamma_correction(color);

  for (int j = 0; j < 24; j++)
  {
    for (int i = 0; i < leds->strip_length(j); i++)
    {
      if (i <= j)
      {

        uint8_t hundrets = j / 100;
        uint8_t tens = (j % 100) / 10;
        uint8_t ones = (j % 10);

        // hundreds - red
        for (int i = 0; i < hundrets; i++)
        {
          leds->set_led(j, i, CRGB(255, 0, 0));
        }

        // tens - green
        for (int i = hundrets; i < hundrets + tens; i++)
        {
          leds->set_led(j, i, CRGB(0, 255, 0));
        }

        // ones - blue
        for (int i = hundrets + tens; i < hundrets + tens + ones; i++)
        {
          leds->set_led(j, i, CRGB(0, 0, 255));
        }
      }
    }
  }
}