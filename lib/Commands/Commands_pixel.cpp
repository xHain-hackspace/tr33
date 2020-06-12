#include <Commands.h>

void Commands::pixel(LedStructure *leds, char *data)
{
  uint8_t strip_index = data[0];
  uint16_t led_index = data[1] << 8 | data[2];
  uint8_t color_index = data[3];

  leds->set_led(strip_index, led_index, ColorFromPalette(currentPalette, color_index));
}

void Commands::pixel_rgb(LedStructure *leds, char *data)
{
  uint8_t strip_index = data[0];
  uint16_t led_index = data[1] << 8 | data[2];
  uint8_t red = data[3];
  uint8_t green = data[4];
  uint8_t blue = data[5];

  leds->set_led(strip_index, led_index, CRGB(red, green, blue));
}