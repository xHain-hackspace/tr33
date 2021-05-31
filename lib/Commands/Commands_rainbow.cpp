#include <Commands.h>

void Commands::rainbow(LedStructure *leds, CommandParams cmd)
{
  Rainbow rainbow = cmd.type_params.rainbow;
  uint8_t beat_brightness = 0;
  uint8_t color_index = 0;
  uint8_t offset = 0;

  uint16_t length_pixel = leds->strip_length(cmd.strip_index);

  float width_ratio = float(max(1, rainbow.rainbow_size)) / 150.0;
  uint16_t width_pixel = float(length_pixel) * width_ratio;

  float wave_size_ratio = float(max(1, rainbow.wave_size)) / 255.0;
  uint16_t wave_size_pixel = float(length_pixel) * wave_size_ratio;

  for (int i = 0; i < length_pixel; i++)
  {
    offset = float(i % wave_size_pixel) / float(wave_size_pixel) * 255.0;
    beat_brightness = beatsin8(cmd.type_params.rainbow.speed, 60, 255, 0, offset);
    color_index = float(i % width_pixel) / float(width_pixel) * 255.00;
    leds->fade_led(cmd, i, color_from_palette(cmd, color_index, beat_brightness));
  }
}