#include <Commands.h>

void Commands::pixel(LedStructure *leds, const CommandParams &cmd)
{
  Pixel pixel = cmd.type_params.pixel;

  leds->fade_led(cmd, pixel.led_index, color_from_palette(cmd, pixel.color));
}

void Commands::pixel_rgb(LedStructure *leds, const CommandParams &cmd)
{
  PixelRGB pixel_rgb = cmd.type_params.pixel_rgb;

  leds->fade_led(cmd, pixel_rgb.led_index, CRGB(pixel_rgb.red, pixel_rgb.green, pixel_rgb.blue));
}