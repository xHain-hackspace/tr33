#include <Arduino.h>
#include <Commands.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);

  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
}

void Commands::initial() {
    // command_buffer[0].type = SINGLE_COLOR;
    // command_buffer[0].data[0] = 0;
    // command_buffer[0].data[1] = 0;
    // command_buffer[0].data[2] = 0;

    // command_buffer[0].type = RAINBOW_SINE;
    // command_buffer[0].data[0] = 10;
    // command_buffer[0].data[1] = 50;
    // command_buffer[0].data[2] = 150;

    command_buffer[0].type = SINGLE_HUE;
    command_buffer[0].data[0] = HUE_BLUE;

    // command_buffer[1].type = PING_PONG;

    // command_buffer[1].data[0] = 3;
    // command_buffer[1].data[1] = 0;
    // command_buffer[1].data[2] = 30;
    // command_buffer[1].data[3] = 30;

    // for(int i=1; i<7; i++) {
    //   command_buffer[i].type = PING_PONG;
    //   command_buffer[i].data[0] = i-1;
    //   command_buffer[i].data[1] = 0;
    //   command_buffer[i].data[2] = 10;
    //   command_buffer[i].data[3] = 30;
    // }
}

void Commands::process(char* command_bin) {
  Command command = *(Command *) command_bin;
  if (command.type == ADD_BALL) {
    add_ball(command.data);
  } else {
    if (command.index < COMMAND_BUFFER_SIZE) {
      command_buffer[command.index] = command;
    }
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

  draw_balls();

  FastLED.show();
}

// -- Set leds ----------------------------------------

void set_trunk_led(int trunk, int led, CRGB color) {
  if(trunk < HW_TRUNK_STRIP_COUNT) {
    trunk_leds[trunk][led] = color;
  } else {
    trunk_leds[trunk-HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT-led-1] = color;
  }
}

CRGB get_trunk_led(int trunk, int led) {
  if(trunk < HW_TRUNK_STRIP_COUNT) {
    return trunk_leds[trunk][led];
  } else {
    return trunk_leds[trunk-HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT-led-1];
  }
}

// strip_index: 0-3 => trunk, 4-10 => branch
void set_led(int strip_index, int led, CRGB color) {
  if(strip_index < TRUNK_STRIP_COUNT) {
    set_trunk_led(strip_index, led, color);
  } else {
    branch_leds[strip_index-TRUNK_STRIP_COUNT][led] = color;
  }
}

CRGB get_led(int strip_index, int led) {
  if(strip_index < TRUNK_STRIP_COUNT) {
    return get_trunk_led(strip_index, led);
  } else {
    return branch_leds[strip_index-TRUNK_STRIP_COUNT][led];
  }
}

// value: min: 0.0, max: 1.0
void fade_led_to_red(int strip_index, int led, float value) {
  CRGB color = get_led(strip_index, led);
  color = color + CRGB(value*255.0, 0, 0) - CRGB(0, value*255.0, value*255.0);
  set_led(strip_index, led, color);
}

// strip_index 0-3 => trunk, 4-10 => branch
int index_strip_length(int strip_index) {
  if(strip_index < TRUNK_STRIP_COUNT) {
    return TRUNK_PIXEL_COUNT;
  } else {
    return BRANCH_PIXEL_COUNT;
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
  for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
    for(int j=0; j< BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE);
    }
  }
}

void Commands::single_color(char * data) {
  for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
    for(int j=0; j< TRUNK_PIXEL_COUNT; j++) {
      set_trunk_led(i, j, CHSV(data[0], data[1], data[2]));
    }
  }
  for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
    for(int j=0; j< BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = CHSV(data[0], data[1], data[2]);
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
  int hue = 0;
  int branch_offset = 50;

  for(int i=0; i<TRUNK_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i, 0, data[0], data[1]) * (max_value-min_value);
    for(int j=0; j<TRUNK_STRIP_COUNT; j++) {
      hue = float(i%data[2])/float(data[2])*255.0;
      set_trunk_led(j, i, CHSV(hue, DEFAULT_SATURATION, value));
    }
  }

  for(int i=0; i<BRANCH_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i+branch_offset, 0, data[0], data[1]) * (max_value-min_value);
    for(int j=0; j<BRANCH_STRIP_COUNT; j++) {
      hue = float(i+branch_offset%data[2])/float(data[2])*255.0;
      branch_leds[j][i] = CHSV(hue, DEFAULT_SATURATION, value);
    }
  }
}

float ball_brightness(int pixel, float center, float width) {
  if (pixel > center-width/2.0 && pixel < center+width/2.0) {
    return 0.5*sinf(2.0*3.1415*((float(pixel)-center)/width+0.25))+0.5;
  } else {
    return 0.0;
  }
}

void render_ball(int strip_index, float center, float width) {
  int strip_length = index_strip_length(strip_index);

  for (int i=0; i<strip_length; i++) {
    float brightness = ball_brightness(i, center, width);
    if (brightness>0) {
      fade_led_to_red(strip_index, i, brightness);
    }
  }
}

// ping_pong effect
float ping_pong_center(float rate, int strip_index) {
  float length = index_strip_length(strip_index);
  float position = float(millis()) / 1000.0 * rate;
  float rem = fmod(position, (length * 2.0));

  if (rem < length) {
    return rem;
  } else {
    return 2.0*(length-1.0)-rem;
  }
}

void Commands::ping_pong(char * data) {
  int strip_index = data[0];
  int hue = data[1];
  int rate = data[2];
  float width = float(data[3]) / 10.0;

  float center = ping_pong_center(rate, strip_index);

  render_ball(strip_index, center, width);
}

// Ball effect
struct Ball {
  int start_millis;

  uint8_t strip_index;
  uint8_t width;
  uint8_t height;
  uint8_t rate;
  uint8_t gravity;

  char data[COMMAND_MAX_DATA];
};

Ball balls[MAX_BALLS];
int next_ball = 0;

float damped_ball_center(float time_seconds, float height, float rate, float gravity) {
  return fabs(sinf(time_seconds*time_seconds*rate/float(50)))*(height-time_seconds*100/gravity);
}

void Commands::add_ball(char * data) {
  Ball ball;
  ball.start_millis = millis();
  ball.strip_index = data[0];
  ball.width = data[2];
  ball.height = data[3];
  ball.rate = data[4];
  ball.gravity = data[5];

  balls[next_ball] = ball;
  if (next_ball++ >= MAX_BALLS) {
    next_ball = 0;
  }
}

void Commands::draw_balls() {
  int now = millis();

  for (int i=0; i<next_ball; i++) {
    float time_seconds = float(now - balls[i].start_millis)/1000.0;
    if (time_seconds < balls[i].gravity/2) {
      float center = damped_ball_center(time_seconds, balls[i].height, balls[i].rate, balls[i].gravity);
      Serial.printf("Index %d Center %f \n", i, center);
      render_ball(balls[i].strip_index, center, float(balls[i].width) / 10.0);
    }
  }
}
