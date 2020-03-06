#include <Commands.h>

void Commands::rainbow_sine(Leds *leds, char *data)
{
  uint8_t strip_index = data[0];
  uint8_t rate = data[1];
  uint8_t wavelength = max(1, int(data[2]));
  uint8_t width_percent = max(1, int(data[3]));
  uint8_t max_brightness = data[4];

  uint8_t brightness = 0;
  uint8_t color_index = 0;
  uint8_t offset = 0;
  uint16_t length = leds->strip_length(strip_index);
  uint16_t width_pixel = float(length) * float(width_percent) / 100.0;

  uint8_t min_brightness = 60;
  if (max_brightness < min_brightness)
  {
    uint8_t tmp = max_brightness;
    max_brightness = min_brightness;
    min_brightness = tmp;
  }

  for (int i = 0; i < length; i++)
  {
    offset = float(i % wavelength) / float(wavelength) * 255.0 * -1.0;
    brightness = beatsin8(rate, min_brightness, max_brightness, 0, offset);
    color_index = float(i % width_pixel) / float(width_pixel) * 255.0;
    leds->set_led(strip_index, i, ColorFromPalette(currentPalette, color_index, brightness));
  }
}