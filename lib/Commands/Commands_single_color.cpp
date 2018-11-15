#include <Commands.h>

void Commands::single_color(char * data) {
  uint8_t strip_index = data[0];
  uint8_t color_index = data[1];
  uint8_t brightness  = data[2];
  
  for (int i=0; i<strip_index_length(strip_index); i++) {
    set_led(strip_index, i, ColorFromPalette(currentPalette, color_index, brightness));
  }
}