#include <Commands.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

CRGBPalette256 currentPalette;

Commands::Commands(void) {
  // todo: use loops
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
  currentPalette = DEFAULT_PALETTE; 

  command_buffer[0].type = SINGLE_COLOR;
  command_buffer[0].data[0] = STRIP_INDEX_ALL;
  command_buffer[0].data[1] = HUE_RED;
  command_buffer[0].data[2] = 255;
}

void Commands::process(char* command_bin) {
  Command command = *(Command *) command_bin;

  switch(command.type) {
    case GRAVITY_EVENT    : gravity_event(); break;
    case SET_PALETTE      : set_palette(command.data); break;
    default               : if (command.index < COMMAND_BUFFER_SIZE) command_buffer[command.index] = command; break;
  }
}

void Commands::run() {
  
  all_off();

  for (int i=0; i<COMMAND_BUFFER_SIZE; i++) {
    switch(command_buffer[i].type) {
      case SINGLE_COLOR      : single_color(command_buffer[i].data); break;
      case RAINBOW_SINE      : rainbow_sine(command_buffer[i].data); break;
      case PING_PONG         : ping_pong(command_buffer[i].data); break;
      case GRAVITY           : gravity(command_buffer[i].data); break;
      case SPARKLE           : sparkle(command_buffer[i].data); break;
      // case SPIRAL            : spiral(command_buffer[i].data); break;
    }
  }

  FastLED.show();
}

//
// -- Set leds ----------------------------------------
// 

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

int Commands::strip_index_length(uint8_t strip_index) {
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

int spiral_strip(uint8_t index) {
  return index % 4;
}
int spiral_led_index(uint8_t index) {
  return index;
}

void Commands::set_led(uint8_t strip_index, int led, CRGB color) {
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

CRGB get_led(uint8_t strip_index, int led) {
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

void Commands::fade_led(uint8_t strip_index, int led, CRGB target, float amount) {
  if (led > 0 && led < strip_index_length(strip_index)) {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(strip_index, led, faded);
  }
}

void Commands::all_off() {
  for(int i=0; i<strip_index_length(STRIP_INDEX_ALL); i++) {
    set_led(STRIP_INDEX_ALL, i, CRGB(0, 0, 0));
  }
  
}

//
// -- Ball rendering ------------------------------------------------
//

void Commands::render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top) {
  switch(ball_type) {
    case BALL_TYPE_SQUARE:
      render_square_ball(strip_index, center, fabs(width), color, ball_brightness);
      break;
    case BALL_TYPE_SINE:
      render_sine_ball(strip_index, center, fabs(width), color, ball_brightness);
      break;
    case BALL_TYPE_COMET:
      render_comet(strip_index, center, width * 10.0, color, bounce_top);
      break;
    case BALL_TYPE_FILL_TOP:
      render_fill_top(strip_index, center, color);
      break;
    case BALL_TYPE_FILL_BOTTOM:
      render_fill_bottom(strip_index, center, color);
      break;
  }
}

void Commands::render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));

  for (int i=start_led; i<=end_led; i++) {
    fade_led(strip_index, i, color, ball_brightness);
  }
}

void Commands::render_sine_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));
  float brightness = 0.0;

  for (int i=start_led; i<=end_led; i++) {
    brightness = 0.5*sinf(2.0*3.1415*((float(i)-center)/width+0.25))+0.5 * ball_brightness;
    if (brightness>0) {
      fade_led(strip_index, i, color, brightness);
    }
  }
}

void Commands::render_comet(uint8_t strip_index, float center, float length, CRGB color, bool bounce_top) {
  render_sine_ball(strip_index, center, 3, color, 1.0);  
  
  if (length != 0) {
    if (length > 0) {
      center = center - 0.5;
    } else {
      center = center + 0.5;
    }

    int strip_length = strip_index_length(strip_index);
    float end = center - length;
    float max_brightness = 0.8;
    float slope = max_brightness / (center - end);

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

void Commands::render_fill_top(uint8_t strip_index, float center, CRGB color) {
  render_sine_ball(strip_index, center, 5, color, 1.0);  

  for (int i = center; i < strip_index_length(strip_index); i++) {
    set_led(strip_index, i, color);
  }
}

void Commands::render_fill_bottom(uint8_t strip_index, float center, CRGB color) {
  render_sine_ball(strip_index, center, 5, color, 1.0);  

  for (int i = 0; i < center; i++) {
    set_led(strip_index, i, color);
  }
}

//
// -- Helper ------------------------------------------------------
//

uint8_t Commands::random_or_value(uint8_t value, uint8_t min, uint8_t max) {
  if (value == 0) {
    return random8(min, max);
  } else {
    return value;
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
    sparkles[sparkle_index].color = CHSV(hue, saturation, 255);
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
