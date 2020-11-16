#include <Commands.h>

void Commands::white(LedStructure *leds, uint8_t *data)
{

  CRGB color = CRGB(255, 255, 255);

  switch (data[0])
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

  for (int i = 0; i < leds->strip_length(leds->strip_index_all()); i++)
  {
    leds->set_led(leds->strip_index_all(), i, color);
  }
}