#include <Arduino.h>
#include <Commands.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
// CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);
}

void Commands::initial() {
    // command_buffer[0].type = SINGLE_HUE;
    // command_buffer[0].data[0] = 0;
    // command_buffer[0].data[1] = 0;
    // command_buffer[0].data[2] = 0;

    command_buffer[0].type = RAINBOW_SINE;
    command_buffer[0].data[0] = 15;
    command_buffer[0].data[1] = 20;

    command_buffer[1].type = PING_PONG;
    command_buffer[1].data[0] = 2;
    command_buffer[1].data[1] = 0;
    command_buffer[1].data[2] = 20;
    command_buffer[1].data[3] = 2;
}

void Commands::process(char* command_bin) {
  Command command = *(Command *) command_bin;
  if (command.index < COMMAND_BUFFER_SIZE) {
    command_buffer[command.index] = command;
  }
}

void Commands::run() {
  for (int i=0; i<COMMAND_BUFFER_SIZE; i++) {
    switch(command_buffer[i].type) {
      case SINGLE_HUE   : single_hue(command_buffer[i].data); break;
      case SINGLE_COLOR : single_color(command_buffer[i].data); break;
      case COLOR_WIPE   : color_wipe(command_buffer[i].data); break;
      case RAINBOW_SINE : rainbow_sine(command_buffer[i].data); break;
      case PING_PONG    : ping_pong(command_buffer[i].data); break;
    }
  }

  FastLED.show();
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

void Commands::single_hue(char * data) {
  for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j< TRUNK_PIXEL_COUNT; j++) {
      set_trunk_led(i, j, CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE));
    }
  }
}

void Commands::single_color(char * data) {
  for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j< TRUNK_PIXEL_COUNT; j++) {
      set_trunk_led(i, j, CHSV(data[0], data[1], data[2]));
    }
  }
}

void Commands::color_wipe(char * data) {
  for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j<((millis()*data[1]/1000) + data[2]) % TRUNK_PIXEL_COUNT; j++) {
      set_trunk_led(i, j, CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE));
    }
  }
}

void Commands::rainbow_sine(char * data) {
  int min_value = 30;
  int max_value = DEFAULT_VALUE;
  int value = 0;

  for(int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j<TRUNK_PIXEL_COUNT; j++) {
      value = min_value + wave_propagation(j, 0, data[0], data[1]) * (max_value-min_value);
      set_trunk_led(i, j, CHSV(j*5, DEFAULT_SATURATION, value));
    }
  }
}

int render_ball(int pixel, int center, int radius) {
  float separation = abs((pixel * PIXEL_DENSITY) - center);
  if (separation < radius) {
    return sinf(2.0*3.1415*((separation+radius)/(float(radius*4.0))))*200.0;
  }
}

int ping_pong_center(float rate, int length) {
  int position = float(millis()) / 1000.0 * rate * float(PIXEL_DENSITY);
  int rem = position % (length * 2);

  if (rem < length) {
    return rem;
  } else {
    return 2*(length-1)-rem;
  }
}

void Commands::ping_pong(char * data) {
  int trunk_index = data[0];
  int hue = data[1];
  int rate = data[2];
  int radius = data[3] * PIXEL_DENSITY / 10;

  int center = ping_pong_center(rate, TRUNK_PIXEL_COUNT * PIXEL_DENSITY);
  int first_pixel = (center / PIXEL_DENSITY) - (radius / PIXEL_DENSITY) - 2;
  int last_pixel = first_pixel + (radius / PIXEL_DENSITY) * 2 + 4;

  // for (int i = first_pixel; i <= last_pixel; i++) {
  for (int i = first_pixel; i <= last_pixel; i++) {
    // set_trunk_led(trunk_index, i, CHSV(0, 255, render_ball(i, ping_pong_center(rate, TRUNK_PIXEL_COUNT), radius)));
    set_trunk_led(trunk_index, i, CHSV(0, 255, render_ball(i, center, radius)));
  }
}
