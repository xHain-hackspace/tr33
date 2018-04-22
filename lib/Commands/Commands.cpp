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

void Commands::init() {
    // command_buffer[0].type = SINGLE_COLOR;
    // command_buffer[0].data[0] = 0;
    // command_buffer[0].data[1] = 0;
    // command_buffer[0].data[2] = 0;

    command_buffer[0].type = RAINBOW_SINE;
    command_buffer[0].data[0] = 10;
    command_buffer[0].data[1] = 30;
    command_buffer[0].data[2] = 150;

    // command_buffer[0].type = SINGLE_HUE;
    // command_buffer[0].data[0] = HUE_BLUE;

    // command_buffer[1].type = PING_PONG_RING;
    // command_buffer[1].data[0] = 0;
    // command_buffer[1].data[1] = 10;
    // command_buffer[1].data[2] = 50;

    // for(int i=1; i<5; i++) {
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
      case SINGLE_HUE        : single_hue(command_buffer[i].data); break;
      case SINGLE_COLOR      : single_color(command_buffer[i].data); break;
      case COLOR_WIPE        : color_wipe(command_buffer[i].data); break;
      case RAINBOW_SINE      : rainbow_sine(command_buffer[i].data); break;
      case PING_PONG         : ping_pong(command_buffer[i].data); break;
      case PING_PONG_RING    : ping_pong_ring(command_buffer[i].data); break;
    }
  }

  draw_balls();

  FastLED.show();
}

void Commands::start_sequence() {
  char data[64];
  data[0] = HUE_BLUE;
  single_hue(data);

  data[0] = HUE_RED;
  data[1] = 10;
  data[2] = 0;
  color_wipe(data);

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

// fade_value: min: 0.0, max: 1.0
void fade_led(int strip_index, int led, CRGB target, float intensity) {
  CRGB current = get_led(strip_index, led);

  int red = current.r + float(target.r-current.r)*intensity;
  int blue = current.b + float(target.b-current.b)*intensity;
  int green = current.g + float(target.g-current.g)*intensity;

  set_led(strip_index, led, CRGB(red, blue, green));
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
  // for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
  //   for(int j=0; j<((millis()*data[1]/1000) + data[2]) % TRUNK_PIXEL_COUNT; j++) {
  //     set_trunk_led(i, j, CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE));
  //   }
  // }
  for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
    for(int j=0; j<((millis()*data[1]/1000) + data[2]) % BRANCH_PIXEL_COUNT; j++) {
      branch_leds[i][j] = CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE);
    }
  }
}

void Commands::rainbow_sine(char * data) {
  int min_value = 30;
  int max_value = DEFAULT_VALUE;
  int value = 0;
  int hue = 0;
  int branch_offset = 50;

  int rate = data[0];
  int wavelength = data[1];
  int width = data[2];
  if (width == 0) width = 1;

  for(int i=0; i<TRUNK_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i, 0, rate, wavelength) * (max_value-min_value);
    for(int j=0; j<TRUNK_STRIP_COUNT; j++) {
      hue = float(i%width)/float(width)*255.0;
      set_trunk_led(j, i, CHSV(hue, DEFAULT_SATURATION, value));
    }
  }

  for(int i=0; i<BRANCH_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i+branch_offset, 0, rate, wavelength) * (max_value-min_value);
    for(int j=0; j<BRANCH_STRIP_COUNT; j++) {
      hue = float(i+branch_offset%width)/float(width)*255.0;
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

void render_ball(int strip_index, float center, float width, int hue) {
  int strip_length = index_strip_length(strip_index);

  for (int i=0; i<strip_length; i++) {
    float brightness = ball_brightness(i, center, width);
    if (brightness>0) {
      fade_led(strip_index, i, CHSV(hue, DEFAULT_SATURATION, DEFAULT_VALUE), brightness);
    }
  }
}

// ping_pong effect
float ping_pong_center(float rate, float length) {
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
  float length = index_strip_length(strip_index);
  float center = ping_pong_center(rate, length);

  render_ball(strip_index, center, width, hue);
}

void Commands::ping_pong_ring(char * data) {
  int hue = data[0];
  int rate = data[1];
  float width = float(data[2]) / 10.0;
  float center = ping_pong_center(rate, BRANCH_PIXEL_COUNT + TRUNK_PIXEL_COUNT);
  float branch_offset = 50.0;

  for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
    render_ball(i, center, width, hue);
  }

  for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
    render_ball(i+TRUNK_STRIP_COUNT, center - branch_offset, width, hue);
  }
}

// Ball effect
struct Ball {
  bool enabled;
  int last_update;
  float position;
  float rate;
  float width;

  uint8_t hue;
  uint8_t strip_index;
  uint8_t gravity;
  uint8_t damping;
};

Ball balls[MAX_BALLS];
int next_ball = 0;

void update_ball(int i) {
  int now = millis();
  int diff = now - balls[i].last_update;
  float interval = float(now - balls[i].last_update)/1000.0;

  balls[i].last_update = now;
  balls[i].position = float(balls[i].position) + float(balls[i].rate) * interval + 0.5 * balls[i].gravity * interval * interval;
  balls[i].rate = balls[i].rate - float(balls[i].gravity) * interval;

  if (balls[i].position < 0) {
    balls[i].enabled = fabs(balls[i].rate) > 12 && diff < 60000;
    balls[i].position = fabs(balls[i].position);
    balls[i].rate = fabs(balls[i].rate) * (1.0 - float(balls[i].damping)/255.0);
  }
}

void Commands::add_ball(char * data) {
  Ball ball;
  ball.enabled = true;
  ball.last_update = millis();
  ball.position = 0;
  ball.strip_index = data[0];
  ball.hue = data[1];
  ball.width = data[2]/10.0;
  ball.rate = data[3];
  ball.gravity = data[4];
  ball.damping = data[5];

  balls[next_ball] = ball;
  if (next_ball++ >= MAX_BALLS) {
    next_ball = 0;
  }
}

void Commands::draw_balls() {
  for (int i=0; i<next_ball; i++) {
    if (balls[i].enabled) {
      update_ball(i);
      render_ball(balls[i].strip_index, balls[i].position, balls[i].width, balls[i].hue);
    }
  }
}
