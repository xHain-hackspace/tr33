#include <Commands.h>

void Commands::rainbow_sine(char * data) {
  uint8_t strip_index     = data[0];
  uint8_t rate            = data[1];
  uint8_t wavelength      = max(1, data[2]);
  uint8_t width_percent   = max(1, data[3]);
  uint8_t max_brightness  = data[4];
  
  uint8_t brightness = 0;
  uint8_t color_index = 0;
  uint8_t offset = 0;
  int strip_length = strip_index_length(strip_index);
  uint16_t width_pixel = float(strip_length) * float(width_percent) / 100.0;

  uint8_t min_brightness = 60;
  if (max_brightness < min_brightness) {
    uint8_t tmp = max_brightness;
    max_brightness = min_brightness;
    min_brightness = tmp;
  }

  for(int i=0; i<strip_length; i++) {
    offset = float(i%wavelength)/float(wavelength)*255.0*-1.0;
    brightness = beatsin8(rate, min_brightness, max_brightness, 0, offset);
    color_index = float(i%width_pixel)/float(width_pixel)*255.0;
    set_led(strip_index, i, ColorFromPalette(currentPalette, color_index, brightness));
  }
}