#include <Commands.h>

CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Command command_buffer[COMMAND_BUFFER_SIZE];

CRGBPalette256 currentPalette;
uint8_t currentMode = MODE_COMMANDS;

Commands::Commands(void) {
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_3>(trunk_leds[2], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_4>(trunk_leds[3], HW_TRUNK_PIXEL_COUNT);

  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_7>(branch_leds[6], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_8>(branch_leds[7], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_9>(branch_leds[8], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_10>(branch_leds[9], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_11>(branch_leds[10], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_12>(branch_leds[11], BRANCH_PIXEL_COUNT);
}

void Commands::init() {
  FastLED.setCorrection(TypicalLEDStrip);
  currentPalette = DEFAULT_PALETTE; 
  
  // uncomment this to show the pin number on each strip
  // show_pin_numbers();

  // command_buffer[0].type = SINGLE_COLOR;
  // command_buffer[0].data[0] = STRIP_INDEX_ALL;
  // command_buffer[0].data[1] = HUE_RED;
  // command_buffer[0].data[2] = 255;  

  command_buffer[0].type = RAINBOW_SINE;
  command_buffer[0].data[0] = STRIP_INDEX_ALL;
  command_buffer[0].data[1] = 30;
  command_buffer[0].data[2] = 100;
  command_buffer[0].data[3] = 100;
  command_buffer[0].data[4] = 255;

  // command_buffer[1].type = PING_PONG;
  // command_buffer[1].data[0] = STRIP_INDEX_ALL;
  // command_buffer[1].data[1] = 1;
  // command_buffer[1].data[2] = 1;
  // command_buffer[1].data[3] = 255;
  // command_buffer[1].data[4] = 190;
  // command_buffer[1].data[5] = 20;
  // command_buffer[1].data[6] = 0;

  // command_buffer[0].type = BEATS;
  // command_buffer[0].data[0] = STRIP_INDEX_ALL_BRANCHES;
  // command_buffer[0].data[1] = 0;
  // command_buffer[0].data[2] = 20;
}

void Commands::process(char* command_bin) {
  Command command = *(Command *) command_bin;

  switch(command.type) {
    case GRAVITY_ADD_BALL : gravity_event(); break;
    case UPDATE_SETTINGS  : update_settings(command.data); break;
    case BEAT             : beat(command.data); break;
    case PIXEL            : pixel(command.data); break;
    case PIXEL_RGB        : pixel_rgb(command.data); break;
    default               : if (command.index < COMMAND_BUFFER_SIZE) command_buffer[command.index] = command; break;
  }
}

void Commands::run() {

  bool run_commands;

  switch(currentMode) {
    case MODE_STREAM  : run_commands = false; break;
    default           : run_commands = true; all_off(); break;
  }

  if(run_commands) {
    for (int i=0; i<COMMAND_BUFFER_SIZE; i++) {
      switch(command_buffer[i].type) {
        case SINGLE_COLOR      : single_color(command_buffer[i].data); break;
        case WHITE             : all_white(); break;
        case RAINBOW_SINE      : rainbow_sine(command_buffer[i].data); break;
        case PING_PONG         : ping_pong(command_buffer[i].data); break;
        case GRAVITY           : gravity(command_buffer[i].data); break;
        case SPARKLE           : sparkle(command_buffer[i].data); break;
        case SHOW_NUMBER       : show_number(command_buffer[i].data); break;
        case RAIN              : rain(command_buffer[i].data); break;
        case BEATS             : beats(command_buffer[i].data); break;
        // case SPIRAL            : spiral(command_buffer[i].data); break;
      }
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
  if (led >= 0 && led < strip_index_length(strip_index)) {
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

void Commands::all_white() {
  for(int i=0; i<strip_index_length(STRIP_INDEX_ALL); i++) {
    set_led(STRIP_INDEX_ALL, i, CRGB(255, 255, 255));
  }  
}

float Commands::ease_in_cubic(float t) {
	return t*t*t;
}

float Commands::ease_out_cubic(float t) {
	t -= 1;
	return t*t*t + 1;
}

float Commands::ease_in_out_cubic(float t) {
	t *= 2;
	if( t < 1 )
		return 0.5f * t*t*t;
	t -= 2;
	return 0.5f*(t*t*t + 2);
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

uint8_t Commands::random_strip(uint8_t strip_index) {
  if (strip_index < BRANCH_STRIP_COUNT + TRUNK_STRIP_COUNT) {
    return strip_index;
  } else if (strip_index == STRIP_INDEX_ALL) {
    return random8(0, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT);
  } else if (strip_index == STRIP_INDEX_ALL_TRUNKS) {
    return random8(0, TRUNK_STRIP_COUNT);
  } else if (strip_index == STRIP_INDEX_ALL_BRANCHES) {
    return random8(TRUNK_STRIP_COUNT, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT);
  }
}


void Commands::show_pin_numbers() {
  Serial.println("Showing pin Numbers");

  command_buffer[0].type = SHOW_NUMBER;
  command_buffer[0].data[0] = 0;
  command_buffer[0].data[1] = TRUNK_PIN_1;
  
  command_buffer[1].type = SHOW_NUMBER;
  command_buffer[1].data[0] = 1;
  command_buffer[1].data[1] = TRUNK_PIN_2;
  
  command_buffer[2].type = SHOW_NUMBER;
  command_buffer[2].data[0] = 2;
  command_buffer[2].data[1] = TRUNK_PIN_3;
  
  command_buffer[3].type = SHOW_NUMBER;
  command_buffer[3].data[0] = 3;
  command_buffer[3].data[1] = TRUNK_PIN_4;
  
  command_buffer[4].type = SHOW_NUMBER;
  command_buffer[4].data[0] = TRUNK_STRIP_COUNT;
  command_buffer[4].data[1] = BRANCH_PIN_1;
  
  command_buffer[5].type = SHOW_NUMBER;
  command_buffer[5].data[0] = TRUNK_STRIP_COUNT + 1;
  command_buffer[5].data[1] = BRANCH_PIN_2;
  
  command_buffer[6].type = SHOW_NUMBER;
  command_buffer[6].data[0] = TRUNK_STRIP_COUNT + 2;
  command_buffer[6].data[1] = BRANCH_PIN_3;
  
  command_buffer[7].type = SHOW_NUMBER;
  command_buffer[7].data[0] = TRUNK_STRIP_COUNT + 3;
  command_buffer[7].data[1] = BRANCH_PIN_4;
  
  command_buffer[8].type = SHOW_NUMBER;
  command_buffer[8].data[0] = TRUNK_STRIP_COUNT + 4;
  command_buffer[8].data[1] = BRANCH_PIN_5;
  
  command_buffer[9].type = SHOW_NUMBER;
  command_buffer[9].data[0] = TRUNK_STRIP_COUNT + 5;
  command_buffer[9].data[1] = BRANCH_PIN_6;
  
  command_buffer[10].type = SHOW_NUMBER;
  command_buffer[10].data[0] = TRUNK_STRIP_COUNT + 6;
  command_buffer[10].data[1] = BRANCH_PIN_7;
  
  command_buffer[11].type = SHOW_NUMBER;
  command_buffer[11].data[0] = TRUNK_STRIP_COUNT + 7;
  command_buffer[11].data[1] = BRANCH_PIN_8;

  command_buffer[12].type = SHOW_NUMBER;
  command_buffer[12].data[0] = TRUNK_STRIP_COUNT + 8;
  command_buffer[12].data[1] = BRANCH_PIN_9;

  command_buffer[13].type = SHOW_NUMBER;
  command_buffer[13].data[0] = TRUNK_STRIP_COUNT + 9;
  command_buffer[13].data[1] = BRANCH_PIN_10;

  command_buffer[14].type = SHOW_NUMBER;
  command_buffer[14].data[0] = TRUNK_STRIP_COUNT + 10;
  command_buffer[14].data[1] = BRANCH_PIN_11;

  command_buffer[15].type = SHOW_NUMBER;
  command_buffer[15].data[0] = TRUNK_STRIP_COUNT + 11;
  command_buffer[15].data[1] = BRANCH_PIN_12;

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
