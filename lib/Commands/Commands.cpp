#include <Arduino.h>
#include <Commands.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
// CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);
}

void Commands::initial_commands() {
    // command_buffer[0].type = SINGLE_HUE;
    // command_buffer[0].data[0] = 0;
    // command_buffer[0].data[1] = 0;
    // command_buffer[0].data[2] = 0;

    command_buffer[0].type = RAINBOW_SINE;
    command_buffer[0].data[0] = 8;
    command_buffer[0].data[1] = 50;
}

void Commands::process_command(char* command_bin) {
  Command command = *(Command *) command_bin;
  if (command.index < COMMAND_BUFFER_SIZE) {
    command_buffer[command.index] = command;
  }
}

// -- trunk strip transformation ----------------------------------------
void set_trunk_led(int trunk, int led, CRGB color) {
  if(trunk < HW_TRUNK_STRIP_COUNT) {
    trunk_leds[trunk][led] = color;
  } else {
    trunk_leds[trunk-HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT-led-1] = color;
  }
}

// -- sine calculation ------------------------------------------------

//returns amplitude modulation term (0...1) based on system time and frequency parameter [1/s]
float amplitude_modulation(float freq_amp_mod){
  if (freq_amp_mod == 0) return 1;
  else return (0.5*(1.0+sinf(freq_amp_mod*2.0*3.1415*float(millis())/1000.0)));
}

//returns wave propgation term (0...1) based on system time and pixel position
//parameter: pixel position, phase offset at pos 0 [px], phase shift with time [px/s], wavelength (size of one wave) [px]
float wave_propagation(float pixel_pos,float phase_offset, float phase_shift_speed, float wavelength){
  return 0.5*(1.0+sinf(2.0*3.1415*(pixel_pos-phase_offset-phase_shift_speed*float(millis())/1000.0)/wavelength));
}

// -- Effects ------------------------------------------------

// void single_hue(char * data) {
//   for(int i=0; i< PIXEL_COUNT; i++) {
//     leds[i] = CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE);
//   }
// }
//
// void single_color(char * data) {
//   for(int i=0; i< PIXEL_COUNT; i++) {
//     leds[i] = CHSV(data[0], data[1], data[2]);
//   }
// }
//
// void color_wipe(char * data) {
//   for(int i=0; i<((millis()*data[1]/1000) + data[2]) % PIXEL_COUNT; i++) {
//     leds[i] = CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE);
//   }
// }

void rainbow_sine(char * data) {
  int min_value = 30;
  int max_value = DEFAULT_VALUE;
  int value = 0;

  for(int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j<TRUNK_PIXEL_COUNT; j++) {
      value = min_value + wave_propagation(j, 0, data[0], data[1]) * (max_value-min_value);
      set_trunk_led(i, j, CHSV(i*5, DEFAULT_SATURATION, value));
    }
  }
}

void Commands::show() {
  for (int i=0; i<COMMAND_BUFFER_SIZE; i++) {
    switch(command_buffer[i].type) {
      // case SINGLE_HUE   : single_hue(command_buffer[i].data); break;
      // case SINGLE_COLOR : single_color(command_buffer[i].data); break;
      // case COLOR_WIPE   : color_wipe(command_buffer[i].data); break;
      case RAINBOW_SINE : rainbow_sine(command_buffer[i].data); break;

    }
  }

  FastLED.show();
}
