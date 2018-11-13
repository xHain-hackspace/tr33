#include <Arduino.h>
#include <Commands.h>
#include <Overrides.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_3>(trunk_leds[2], HW_TRUNK_PIXEL_COUNT);

  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);

}

void Commands::init() {
    // command_buffer[0].type = SINGLE_COLOR;
    // command_buffer[0].data[0] = 0;
    // command_buffer[0].data[1] = 0;
    // command_buffer[0].data[2] = 0;

    // command_buffer[0].type = RAINBOW_SINE;
    // command_buffer[0].data[0] = 10;
    // command_buffer[0].data[1] = 100;
    // command_buffer[0].data[2] = 100;
    // command_buffer[0].data[3] = 255;

    command_buffer[0].type = SINGLE_HUE;
    command_buffer[0].data[0] = STRIP_INDEX_ALL;
    command_buffer[0].data[1] = HUE_RED;

    // command_buffer[0].type = SPIRAL;

    // command_buffer[0].type = COLOR_WIPE;
    // command_buffer[0].data[0] = STRIP_INDEX_ALL_BRANCHES;
    // command_buffer[0].data[1] = HUE_BLUE;
    // command_buffer[0].data[2] = 50;

    // command_buffer[1].type = GRAVITY;
    // command_buffer[1].data[0] = 11;
    // command_buffer[1].data[1] = 0;
    // command_buffer[1].data[2] = 25;
    // command_buffer[1].data[3] = 100;
    // command_buffer[1].data[4] = 2;

    // command_buffer[1].type = SPARKLE;
    // command_buffer[1].data[0] = 0;
    // command_buffer[1].data[1] = 255;
    // command_buffer[1].data[2] = 15;
    // command_buffer[1].data[3] = 10;

    // command_buffer[1].type = PING_PONG;
    // command_buffer[1].data[0] = STRIP_INDEX_ALL_TRUNKS;
    // command_buffer[1].data[1] = BALL_TYPE_COMET;
    // command_buffer[1].data[2] = 0;
    // command_buffer[1].data[3] = 0;
    // command_buffer[1].data[4] = 15;
    // command_buffer[1].data[5] = 80;
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
  
  char args[1] = {0};
  off(args);

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
      // case SPIRAL            : spiral(command_buffer[i].data); break;
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

int strip_index_length(int strip_index) {
  if(strip_index < TRUNK_STRIP_COUNT || strip_index == STRIP_INDEX_ALL_TRUNKS) {
    return TRUNK_PIXEL_COUNT;
  } else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT || strip_index == STRIP_INDEX_ALL_BRANCHES) {
    return BRANCH_PIXEL_COUNT;
  } else if (strip_index == STRIP_INDEX_ALL) {
    return TRUNK_PIXEL_COUNT + BRANCH_PIXEL_COUNT;
  } else if (strip_index == STRIP_INDEX_SPIRAL) {
    return 100;
  }
}

int spiral_strip(int index) {
  return index % 4;
}
int spiral_led_index(int index) {
  return index;
}

void set_led(int strip_index, int led, CRGB color) {
  if (led > 0 && led < strip_index_length(strip_index)) {
    // single trunk
    if (strip_index < TRUNK_STRIP_COUNT) {
      set_trunk_led(strip_index, led, color);
    // single branch
    } else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT){
      branch_leds[strip_index-TRUNK_STRIP_COUNT][led] = color;
    // all trunks
    } else if (strip_index == STRIP_INDEX_ALL_TRUNKS) {
      for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
        set_trunk_led(i, led, color);
      }
    // all branches
    } else if (strip_index == STRIP_INDEX_ALL_BRANCHES) {
      for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
        branch_leds[i][led] = color;
      }
    // all trunks and branches
    } else if (strip_index == STRIP_INDEX_ALL) {
      if (led < TRUNK_PIXEL_COUNT) {
        for (int i=0; i<TRUNK_STRIP_COUNT; i++) {
          set_trunk_led(i, led, color);
        }
      } else {
        for (int i=0; i<BRANCH_STRIP_COUNT; i++) {
          branch_leds[i][led-TRUNK_PIXEL_COUNT] = color;
        }
      }
    // spiral
    } else if (strip_index == STRIP_INDEX_SPIRAL) {
      set_trunk_led(spiral_strip(led), spiral_led_index(led), color);
    } 
  }
}


CRGB get_led(int strip_index, int led) {
  // single trunk
  if (strip_index < TRUNK_STRIP_COUNT) {
    return get_trunk_led(strip_index, led);
  // single branch
  } else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT){
    return branch_leds[strip_index-TRUNK_STRIP_COUNT][led];
  // all trunks
  } else if (strip_index == STRIP_INDEX_ALL_TRUNKS) {
    return get_trunk_led(0, led);
  // all branches
  } else if (strip_index == STRIP_INDEX_ALL_BRANCHES) {
    return branch_leds[0][led];
  // all trunks and branches
  } else if (strip_index == STRIP_INDEX_ALL) {
    if (led < TRUNK_PIXEL_COUNT) {
      return get_trunk_led(0, led);
    } else {
      return branch_leds[0][led - TRUNK_PIXEL_COUNT];
    }
  // spiral
  } else if (strip_index == STRIP_INDEX_SPIRAL) {
    return get_trunk_led(spiral_strip(led), spiral_led_index(led));
  } 
}

// fade_value: min: 0.0, max: 1.0
void fade_led(int strip_index, int led, CRGB target, float intensity) {
  if (led > 0 && led < strip_index_length(strip_index)) {
    CRGB current = get_led(strip_index, led);

    int red = current.r + float(target.r-current.r)*intensity;
    int blue = current.b + float(target.b-current.b)*intensity;
    int green = current.g + float(target.g-current.g)*intensity;

    set_led(strip_index, led, CRGB(red, green, blue));
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
  int strip_index = data[0];
  int hue = data[1];

  for (int i=0; i<strip_index_length(strip_index); i++) {
    set_led(strip_index, i, CHSV(hue, DEFAULT_SATURATION, DEFAULT_VALUE));
  }
}

void Commands::single_color(char * data) {
  int strip_index = data[0];
  int hue = data[1];
  int saturation = data[2];
  int value = data[3];

  for (int i=0; i<strip_index_length(strip_index); i++) {
    set_led(strip_index, i, CHSV(hue, saturation, value));
  }
}

void Commands::color_wipe(char * data) {
  int strip_index = data[0];
  int hue = data[1];
  int rate = data[2]; // pixel/second
  int offset = data[3]; // pixel

  for(int i=0; i<((millis()*rate/1000) + offset) % (strip_index_length(strip_index)); i++) {
    set_led(strip_index, i, CHSV(hue, DEFAULT_SATURATION, DEFAULT_VALUE));
  }
}

void Commands::off(char * data) {
  char args[4] = {STRIP_INDEX_ALL, 0,0, 0};
  single_color(args);
}

void Commands::white(char * data) {
  char value = data[1];
  char args[4] = {STRIP_INDEX_ALL, 0, 0, value};
  single_color(args);
}

void Commands::rainbow_sine(char * data) {
  int value = 0;
  int hue = 0;

  int strip_index = data[0];
  int rate = data[1];
  int wavelength = data[2];
  int width_percent = max(1, data[3]);
  int max_value = data[4];
  int min_value = 75;

  int width_pixel = float(strip_index_length(strip_index)) * float(width_percent) / 100.0;

  for(int i=0; i<strip_index_length(strip_index); i++) {
    value = min_value + wave_propagation(i, 0, rate, wavelength) * (max_value-min_value);
    hue = float(i%width_pixel)/float(width_pixel)*255.0;
    set_led(strip_index, i, CHSV(hue, DEFAULT_SATURATION, value));
  }
}


// ball rendering
void render_square_ball(int strip_index, float center, float width, CHSV color, float ball_intensity) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));

  for (int i=start_led; i<=end_led; i++) {
    fade_led(strip_index, i, color, ball_intensity);
  }
}

void render_sine_ball(int strip_index, float center, float width, CHSV color, float ball_intensity) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));

  for (int i=start_led; i<=end_led; i++) {
    float led_intensity = 0.5*sinf(2.0*3.1415*((float(i)-center)/width+0.25))+0.5 * ball_intensity;
    if (led_intensity>0) {
      fade_led(strip_index, i, color, led_intensity);
    }
  }
}

void render_tail(int strip_index, float center, float length, CHSV color, bool bounce_top) {
  if (length != 0) {
    if (length > 0) {
      center = center - 0.5;
    } else {
      center = center + 0.5;
    }

    int strip_length = strip_index_length(strip_index);
    float end = center - length;
    float max_brightness = 1;
    float slope = max_brightness / (center - end);
    color.saturation = color.saturation - 20;
    color.value = color.value - 50;

    int first_led = min(floorf(center), floorf(end));
    int last_led = max(ceilf(center), ceilf(end));

    for (int i=first_led; i<last_led; i++) {
      float intensity = slope * (float(i) - end);
      if (intensity <= max_brightness && intensity > 0) {
        int led_index = i;
        if (led_index < 0) led_index = -1*led_index;
        if (bounce_top) {
          if (led_index >= strip_length) led_index = 2*(strip_length)-led_index;
        }
        fade_led(strip_index, led_index, color, intensity);
      }
    }
  }
}

void render_ball(int strip_index, int ball_type, float center, float width, CHSV color, float ball_intensity, bool bounce_top) {
  switch(ball_type) {
    case BALL_TYPE_SQUARE:
      render_square_ball(strip_index, center, fabs(width), color, ball_intensity);
      break;
    case BALL_TYPE_SINE:
      render_sine_ball(strip_index, center, fabs(width), color, ball_intensity);
      break;
    case BALL_TYPE_COMET:
      render_tail(strip_index, center, width * 10.0, color, bounce_top);
      render_sine_ball(strip_index, center, 3, color, ball_intensity);
      break;
  }
}

// ping_pong effect
void Commands::ping_pong(char * data) {
  int strip_index = data[0];
  int ball_type = data[1];
  float rel_offset = float(data[2]) / 100.0;
  int hue = data[3];
  int bpm = data[4];
  float width = float(data[5]) / 10.0;

  float length = strip_index_length(strip_index);
  float offset = rel_offset * length * 2;
  float rate = length / (60.0 / bpm);
  float total_distance = offset + float(millis()) / 1000.0 * rate;
  float center = fmod(total_distance, (length * 2.0));
  CHSV color = CHSV(hue, DEFAULT_SATURATION, DEFAULT_VALUE);

  if(center > length) {
    center = 2.0*(length-1.0)-center;
    width = -1.0 * width;
  }

  render_ball(strip_index, ball_type, center, width, color, 1.0, true);
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
  float width;  // deprecated

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
  // gravity_width = data[2];
  gravity_rate = data[2];
  int frequency = data[3];

  if (frequency > 0 && gravity_last_ball < millis() && 10000 / (millis() - gravity_last_ball) < frequency){
    gravity_event();
  }

  for (int i=0; i<MAX_GRAVITY_BALLS; i++) {
    if (balls[i].enabled) {
      update_ball(i);
      CHSV color = CHSV(balls[i].hue, DEFAULT_SATURATION, DEFAULT_VALUE);  
      float width = balls[i].rate/50.0;
      render_ball(balls[i].strip_index, BALL_TYPE_COMET, balls[i].position, width, color, 1.0, false);
    }
  }
}

// sparkle effect
uint8_t sparkle_index = 0;

struct Sparkle {
  bool enabled;
  CHSV color;
  float width;
  float brightness;
  uint8_t strip_index;
  int center;
  int start_time;
};

Sparkle sparkles[MAX_SPARKLES];

uint8_t random_strip(uint8_t strip_index) {
  if (strip_index < BRANCH_STRIP_COUNT + TRUNK_STRIP_COUNT) {
    return strip_index;
  } else if (strip_index == STRIP_INDEX_ALL) {
    return random8(0, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT - 1);
  } else if (strip_index == STRIP_INDEX_ALL_TRUNKS) {
    return random8(0, TRUNK_STRIP_COUNT - 1);
  } else if (strip_index == STRIP_INDEX_ALL_BRANCHES) {
    return random8(TRUNK_STRIP_COUNT, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT - 1);
  }
}

void Commands::sparkle(char * data) {
  uint8_t strip_index = data[0];
  uint8_t hue = random_or_value(data[1], 0, 255);
  uint8_t saturation = data[2];
  float width = float(random_or_value(data[3], 0, 255))/10.0;
  uint8_t frequency = data[4];  // sparkles per seconds
  int now = millis();

  if (frequency > 0 && (1000 / (now - sparkles[sparkle_index].start_time)) < frequency){
    if (sparkle_index ++ >= MAX_SPARKLES) {
      sparkle_index = 0;
    }
    sparkles[sparkle_index].enabled = true;
    sparkles[sparkle_index].color = CHSV(hue, saturation, DEFAULT_VALUE);
    sparkles[sparkle_index].width = width;
    sparkles[sparkle_index].brightness = float(random(10))/20.0 + 0.5;
    sparkles[sparkle_index].strip_index = random_strip(strip_index);
    sparkles[sparkle_index].center = random(0, strip_index_length(sparkles[sparkle_index].strip_index)-1);
    sparkles[sparkle_index].start_time = now;
  }

  for (int i=0; i<MAX_SPARKLES; i++) {
    if (sparkles[i].enabled) {
      float brightness = sparkles[i].brightness - float(now - sparkles[i].start_time) / (100.0 * float(SPARKLES_DIM_RATE));
      if (brightness > 0) {
        render_ball(sparkles[i].strip_index, BALL_TYPE_SINE, sparkles[i].center, sparkles[i].width, sparkles[i].color, brightness, false);
      } else {
        sparkles[i].enabled = false;
      }
    }
  }
}

// // spiral

// void Commands::spiral(char * data) {
//   int hue = 0;
//   float slope = 3.0;
//   int strip_index = STRIP_INDEX_ALL_TRUNKS;

//   float current_height = 0.0;
//   int current_strip = 0;

//   while (current_height <= strip_index_length(strip_index)) {
//     // render_ball(current_strip, current_height, 1, CHSV(0, DEFAULT_SATURATION, DEFAULT_VALUE), 1);
//     current_height = current_height + slope;
//     current_strip = current_strip + 1;
//     if (current_strip > 3) {
//       current_strip = 0;
//     }
//   }

// }