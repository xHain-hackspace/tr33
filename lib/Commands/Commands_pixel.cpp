#include <Commands.h>

void Commands::pixel(char * data) {
  uint8_t strip_index = data[0];
  uint8_t led_index = data[1];
  uint8_t color_index = data[2];

  set_led(strip_index, led_index, ColorFromPalette(currentPalette, color_index));
}

void Commands::pixel_rgb(char * data) {
  uint8_t strip_index = data[0];
  uint8_t led_index = data[1];
  uint8_t red = data[3];
  uint8_t green = data[2];
  uint8_t blue = data[4];

  set_led(strip_index, led_index, CRGB(red, green, blue));
}