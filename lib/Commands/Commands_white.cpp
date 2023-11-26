#include <Commands.h>

void Commands::white(LedStructure *leds, const CommandParams &cmd)
{
  CRGB color = CRGB(255, 255, 255);

  switch (cmd.type_params.white.color_temperature)
  {
  case 1:
    color = CRGB(0xE1A024);
    break;
  case 2:
    color = CRGB::FairyLight;
    break;
  case 3:
    color = CRGB::FairyLightNCC;
    break;
  case 4:
    color = CRGB::AntiqueWhite;
    break;
  case 5:
    color = CRGB::Wheat;
    break;
  }

  for (int i = 0; i < leds->strip_length(cmd.strip_index); i++)
  {
    leds->fade_led(cmd, i, color);
  }
}