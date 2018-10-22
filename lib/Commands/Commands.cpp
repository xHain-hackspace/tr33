#include <Arduino.h>
#include <Commands.h>
#include <Overrides.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);

  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);
}

void Commands::init() {
    command_buffer[0].type = SINGLE_COLOR;
    command_buffer[0].data[0] = 0;
    command_buffer[0].data[1] = 0;
    command_buffer[0].data[2] = 0;

    // command_buffer[0].type = RAINBOW_SINE;
    // command_buffer[0].data[0] = 10;
    // command_buffer[0].data[1] = 100;
    // command_buffer[0].data[2] = 100;
    // command_buffer[0].data[3] = 255;

    // command_buffer[0].type = SINGLE_HUE;
    // command_buffer[0].data[0] = HUE_RED;

    // command_buffer[1].type = COLOR_WIPE;
    // command_buffer[1].data[0] = HUE_BLUE;
    // command_buffer[1].data[1] = 50;

    command_buffer[1].type = GRAVITY;
    command_buffer[1].data[0] = 11;
    command_buffer[1].data[1] = 0;
    command_buffer[1].data[2] = 25;
    command_buffer[1].data[3] = 100;
    command_buffer[1].data[4] = 2;

    // command_buffer[1].type = SPARKLE;
    // command_buffer[1].data[0] = 0;
    // command_buffer[1].data[1] = 255;
    // command_buffer[1].data[2] = 15;
    // command_buffer[1].data[3] = 10;

    // command_buffer[1].type = PING_PONG;
    // command_buffer[1].data[0] = 10;
    // command_buffer[1].data[1] = 0;
    // command_buffer[1].data[2] = 10;
    // command_buffer[1].data[3] = 50;
    //
    // command_buffer[2].type = PING_PONG;
    // command_buffer[2].data[0] = 10;
    // command_buffer[2].data[1] = 0;
    // command_buffer[2].data[2] = 13;
    // command_buffer[2].data[3] = 50;
    //
    // command_buffer[3].type = PING_PONG;
    // command_buffer[3].data[0] = 10;
    // command_buffer[3].data[1] = 0;
    // command_buffer[3].data[2] = 15;
    // command_buffer[3].data[3] = 50;

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
  if (command.type == GRAVITY_EVENT) {
    gravity_event();
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
      case GRAVITY           : gravity(command_buffer[i].data); break;
      case OFF               : off(command_buffer[i].data); break;
      case WHITE             : white(command_buffer[i].data); break;
      case SPARKLE           : sparkle(command_buffer[i].data); break;
    }
  }

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

void set_led(int strip_index, int led, CRGB color) {
  int total_count = TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT;
  
  // single trunk
  if (strip_index < TRUNK_STRIP_COUNT) {
    set_trunk_led(strip_index, led, color);
  // single branch
  } else if (strip_index < total_count){
    branch_leds[strip_index-TRUNK_STRIP_COUNT][led] = color;
  // all trunks
  } else if (strip_index == total_count) {
    for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
      set_trunk_led(i, led, color);
    }
  // all branches
  } else if (strip_index == total_count + 1) {
    for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
      branch_leds[i][led] = color;
    }
  // all trunks and branches
  } else if (strip_index == total_count + 2) {
    if (led < TRUNK_PIXEL_COUNT) {
      for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
        set_trunk_led(i, led, color);
      }
    } else {
      for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
        branch_leds[i][led-TRUNK_PIXEL_COUNT] = color;
      }
    }
  }
}

CRGB get_led(int strip_index, int led) {
  int total_count = TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT;

  // single trunk
  if (strip_index < TRUNK_STRIP_COUNT) {
    return get_trunk_led(strip_index, led);
  // single branch
  } else if (strip_index < total_count){
    return branch_leds[strip_index-TRUNK_STRIP_COUNT][led];
  // all trunks
  } else if (strip_index == total_count) {
    return get_trunk_led(0, led);
  // all branches
  } else if (strip_index == total_count + 1) {
    return branch_leds[0][led];
  // all trunks and branches
  } else if (strip_index == total_count + 2) {
    if (led < TRUNK_PIXEL_COUNT) {
      return get_trunk_led(0, led);
    } else {
      return branch_leds[0][led - TRUNK_PIXEL_COUNT];
    }
  }
}

// fade_value: min: 0.0, max: 1.0
void fade_led(int strip_index, int led, CRGB target, float intensity) {
  CRGB current = get_led(strip_index, led);

  int red = current.r + float(target.r-current.r)*intensity;
  int blue = current.b + float(target.b-current.b)*intensity;
  int green = current.g + float(target.g-current.g)*intensity;

  set_led(strip_index, led, CRGB(red, green, blue));
}


int strip_index_length(int strip_index) {
  int total_count = TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT;

  if(strip_index < TRUNK_STRIP_COUNT || strip_index == total_count) {
    return TRUNK_PIXEL_COUNT;
  } else if (strip_index < total_count || strip_index == total_count + 1){
    return BRANCH_PIXEL_COUNT;
  } else {
    return total_count;
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
  for(int j=0; j<((millis()*data[1]/1000) + data[2]) % (TRUNK_PIXEL_COUNT+BRANCH_PIXEL_COUNT); j++) {
    if (j < TRUNK_PIXEL_COUNT) {
      for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
        set_trunk_led(i, j, CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE));
      }
    } else {
      for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
        branch_leds[i][j-TRUNK_PIXEL_COUNT] = CHSV(data[0], DEFAULT_SATURATION, DEFAULT_VALUE);
      }
    }
  }
}

void Commands::off(char * data) {
  char args[3] = {0,0,0};
  single_color(args);
}

void Commands::white(char * data) {
  char args[3] = {0,0,data[1]};
  single_color(args);
}

void Commands::rainbow_sine(char * data) {
  int value = 0;
  int hue = 0;

  int rate = data[0];
  int wavelength = data[1];
  int width_percent = max(1, data[2]);
  int min_value = 75;
  int max_value = data[3];

  int width_pixel = float(BRANCH_OFFSET + BRANCH_PIXEL_COUNT) * float(width_percent) / 100.0;

  for(int i=0; i<TRUNK_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i, 0, rate, wavelength) * (max_value-min_value);
    for(int j=0; j<TRUNK_STRIP_COUNT; j++) {
      hue = float(i%width_pixel)/float(width_pixel)*255.0;
      set_trunk_led(j, i, CHSV(hue, DEFAULT_SATURATION, value));
    }
  }

  for(int i=0; i<BRANCH_PIXEL_COUNT; i++) {
    value = min_value + wave_propagation(i+BRANCH_OFFSET, 0, rate, wavelength) * (max_value-min_value);
    for(int j=0; j<BRANCH_STRIP_COUNT; j++) {
      hue = float(i+BRANCH_OFFSET%width_pixel)/float(width_pixel)*255.0;
      branch_leds[j][i] = CHSV(hue, DEFAULT_SATURATION, value);
    }
  }
}


// ping_pong effect
float ball_brightness(int pixel, float center, float width) {
  if (pixel > center-width/2.0 && pixel < center+width/2.0) {
    return 0.5*sinf(2.0*3.1415*((float(pixel)-center)/width+0.25))+0.5;
  } else {
    return 0.0;
  }
}

void render_ball(int strip_index, float center, float width, CRGB color, float brightness) {
  int strip_length = strip_index_length(strip_index);

  for (int i=0; i<strip_length; i++) {
    float led_brightness = ball_brightness(i, center, width) * brightness;
    if (led_brightness>0) {
      fade_led(strip_index, i, color, led_brightness);
    }
  }
}

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
  float length = strip_index_length(strip_index);
  float center = ping_pong_center(rate, length);

  render_ball(strip_index, center, width, CHSV(hue, DEFAULT_SATURATION, DEFAULT_VALUE), 1);
}

// Gravity effect
uint8_t gravity_hue = 0;
uint8_t gravity_width = 0;
uint8_t gravity_rate = 0;
uint8_t gravity_strip_index = 0;
int gravity_last_ball = 0;

struct Ball {
  bool enabled;
  int start;
  int last_update;
  float position;
  float rate;
  float width;

  uint8_t hue;
  uint8_t strip_index;
};

Ball balls[MAX_GRAVITY_BALLS];
int next_ball = 0;

void update_ball(int i) {
  int now = millis();
  float interval = float(now - balls[i].last_update)/1000.0;

  balls[i].last_update = now;
  balls[i].position = float(balls[i].position) + float(balls[i].rate) * interval + 0.5 * float(GRAVITY_VALUE) * interval * interval;
  balls[i].rate = balls[i].rate - float(GRAVITY_VALUE) * interval;

  if (balls[i].position < 0) {
    balls[i].enabled = fabs(balls[i].rate) > 20 && now - balls[i].start < 40000;
    balls[i].position = fabs(balls[i].position);
    balls[i].rate = fabs(balls[i].rate) * (1.0 - GRAVITY_DAMPING/255.0);
  }
}

uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max) {
  if (value == 0) {
    return random8(min, max);
  } else {
    return value;
  }
}

void Commands::gravity_event() {
  Ball ball;
  ball.enabled = true;
  ball.last_update = millis();
  ball.start = millis();
  ball.position = 0;

  ball.strip_index = gravity_strip_index;
  ball.width = float(random_or_value(gravity_width, 0, 255))/10.0;
  ball.rate = random_or_value(gravity_rate, 30, 175);
  ball.hue = random_or_value(gravity_hue, 0, 255);

  gravity_last_ball = millis();

  balls[next_ball] = ball;
  next_ball++;
  if (next_ball >= MAX_GRAVITY_BALLS) {
    next_ball = 0;
  }
}

void Commands::gravity(char * data) {
  gravity_strip_index = data[0];
  gravity_hue = data[1];
  gravity_width = data[2];
  gravity_rate = data[3];
  int frequency = data[4];

  if (frequency > 0 && 10000 / (millis() - gravity_last_ball) < frequency){
    gravity_event();
  }

  for (int i=0; i<MAX_GRAVITY_BALLS; i++) {
    if (balls[i].enabled) {
      update_ball(i);
      CRGB color = CHSV(balls[i].hue, DEFAULT_SATURATION, DEFAULT_VALUE);
      render_ball(balls[i].strip_index, balls[i].position, balls[i].width, color, 1);
    }
  }
}

// sparkle effect
uint8_t dim_rate = 50;
uint8_t sparkle_index = 0;

struct Sparkle {
  bool enabled;
  CRGB color;
  float width;
  float brightness;
  uint8_t strip_index;
  int center;
  int start_time;
};

Sparkle sparkles[MAX_SPARKLES];

void Commands::sparkle(char * data) {
  uint8_t hue = random_or_value(data[0], 0, 255);
  uint8_t saturation = data[1];
  float width = float(random_or_value(data[2], 0, 255))/10.0;
  uint8_t frequency = data[3];  // sparkles per seconds
  int now = millis();

  if (frequency > 0 && (1000 / (now - sparkles[sparkle_index].start_time)) < frequency){
    if (sparkle_index ++ >= MAX_SPARKLES) {
      sparkle_index = 0;
    }
    sparkles[sparkle_index].enabled = true;
    sparkles[sparkle_index].color = CHSV(hue, saturation, DEFAULT_VALUE);
    sparkles[sparkle_index].width = width;
    sparkles[sparkle_index].brightness = float(random(10))/20.0 + 0.5;
    sparkles[sparkle_index].strip_index = random8(TRUNK_STRIP_COUNT, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT - 1);
    sparkles[sparkle_index].center = random(0, strip_index_length(sparkles[sparkle_index].strip_index)-1);
    sparkles[sparkle_index].start_time = now;
  }

  for (int i=0; i<MAX_SPARKLES; i++) {
    if (sparkles[i].enabled) {
      float brightness = sparkles[i].brightness - float(now - sparkles[i].start_time) / (100.0 * float(dim_rate));

      if (brightness > 0) {
        render_ball(sparkles[i].strip_index, sparkles[i].center, sparkles[i].width,sparkles[i].color , brightness);
      } else {
        sparkles[i].enabled = false;
      }
    }
  }
}
