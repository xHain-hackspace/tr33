#include <Arduino.h>
#include <FastLED.h>

// trunk config
#define TRUNK_PIN_1 23
#define TRUNK_PIN_2 22
#define TRUNK_PIN_3 14
#define TRUNK_PIN_4 32
#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 4
#define TRUNK_PIXEL_COUNT HW_TRUNK_PIXEL_COUNT / 2
#define TRUNK_STRIP_COUNT HW_TRUNK_STRIP_COUNT * 2

// branch config
#define BRANCH_PIN_1 15
#define BRANCH_PIN_2 33
#define BRANCH_PIN_3 27
#define BRANCH_PIN_4 12u
#define BRANCH_PIN_5 21
#define BRANCH_PIN_6 19
#define BRANCH_PIN_7 18
#define BRANCH_PIN_8 5
#define BRANCH_PIN_9 4
#define BRANCH_PIN_10 25
#define BRANCH_PIN_11 26
#define BRANCH_PIN_12 13
#define BRANCH_PIXEL_COUNT 90
#define BRANCH_STRIP_COUNT 12
#define BRANCH_OFFSET 50

// strip index
#define STRIP_INDEX_ALL_TRUNKS TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT
#define STRIP_INDEX_ALL_BRANCHES TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 1
#define STRIP_INDEX_ALL TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 2
#define STRIP_INDEX_SPIRAL TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 3

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70
#define MAX_SPARKLES 500
#define SPARKLES_DIM_RATE 40
#define MAX_RAIN_DROPS 500

// -- COMMANDS ---------------------------------------------------------------------------

#define DISABLE 0
#define SINGLE_COLOR 1
#define WHITE 2
#define RAINBOW_SINE 3
#define PING_PONG 4
#define GRAVITY 5
#define SPARKLE 6
#define SHOW_NUMBER 7
#define RAIN 8
#define BEATS 9
#define MAPPED_SWIPE 10
#define MAPPED_SHAPE 11

// -- EVENTS ------------------------------------------------------------------------------

#define GRAVITY_ADD_BALL 100
#define UPDATE_SETTINGS 101
#define BEAT 102
#define PIXEL 103
#define PIXEL_RGB 104

// -- BALL_TYPES --------------------------------------------------------------------------

#define BALL_TYPE_SQUARE 0
#define BALL_TYPE_SINE 1
#define BALL_TYPE_COMET 2
#define BALL_TYPE_NYAN 3
#define BALL_TYPE_FILL_TOP 4
#define BALL_TYPE_FILL_BOTTOM 5

// -- MAPPED -----------------------------------------------------------------------

#define SWIPE_TOP_BOTTOM 0
#define SWIPE_BOTTOM_TOP 1
#define SWIPE_LEFT_RIGHT 2
#define SWIPE_RIGHT_LEFT 3

#define SHAPE_SQUARE 0
#define SHAPE_HOLLOW_SQUARE 1
#define SHAPE_CIRCLE 2

struct Command;

class Tr33
{

public:
  Tr33();
  void init();
  void all_off();
  void process_event(Command *command);
  void process_command(Command *command);

private:
  // commands - rendered on each loop
  void single_color(char *data);
  void rainbow_sine(char *data);
  void ping_pong(char *data);
  void gravity(char *data);
  void sparkle(char *data);
  void show_number(char *data);
  void rain(char *data);
  void beats(char *data);
  void mapped_swipe(char *data);
  void mapped_shape(char *data);

  // events - rendered once
  void gravity_event();
  void update_settings(char *data);
  void beat(char *data);
  void pixel(char *data);
  void pixel_rgb(char *data);

  // set leds
  void all_white();
  void set_led(uint8_t strip_index, int led, CRGB color);
  void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  int strip_index_length(uint8_t strip_index);

  // ball rendering
  void render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
  void render_sine_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
  void render_comet(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_nyan(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_fill_top(uint8_t strip_index, float center, CRGB color, float ball_brightness);
  void render_fill_bottom(uint8_t strip_index, float center, CRGB color, float ball_brightness);

  // mapped rendering
  void render_mapped_square(float x, float y, float size, CRGB color, bool fill);
  void render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz);

  // debugging
  void show_pin_numbers();

  // helper
  uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max);
  uint8_t random_strip(uint8_t strip_index);

  // float easing functions
  float ease_in_cubic(float t);
  float ease_out_cubic(float t);
  float ease_in_out_cubic(float t);
};
