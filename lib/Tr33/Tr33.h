#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

// trunk config
#define TRUNK_PIN_1 23
#define TRUNK_PIN_2 23
#define TRUNK_PIN_3 23
#define TRUNK_PIN_4 23
#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 4
#define TRUNK_PIXEL_COUNT HW_TRUNK_PIXEL_COUNT / 2
#define TRUNK_STRIP_COUNT HW_TRUNK_STRIP_COUNT * 2

// branch config
#define BRANCH_PIN_1 33
#define BRANCH_PIN_2 32
#define BRANCH_PIN_3 25
#define BRANCH_PIN_4 27
#define BRANCH_PIN_5 14
#define BRANCH_PIN_6 13
#define BRANCH_PIN_7 15
#define BRANCH_PIN_8 22
#define BRANCH_PIN_9 12
#define BRANCH_PIN_10 4
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
#define MAX_RAIN_DROPS 500

// -- BALL_TYPES --------------------------------------------------------------------------

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
struct Leds;

class Tr33 : public Leds
{

public:
  Tr33();
  void init();
  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint8_t strip_count();

  // commands - rendered on each loop
  void all_white();
  void rainbow_sine(char *data);
  void ping_pong(char *data);
  void gravity(char *data);
  void beats(char *data);
  void mapped_swipe(char *data);
  void mapped_shape(char *data);
  void twang(char *data);

  // events - rendered once
  void gravity_event();
  void beat(char *data);
  void joystick(char *data);

private:
  // ball rendering
  void render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
  void render_comet(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_nyan(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  void render_fill_top(uint8_t strip_index, float center, CRGB color, float ball_brightness);
  void render_fill_bottom(uint8_t strip_index, float center, CRGB color, float ball_brightness);

  // mapped rendering
  void render_mapped_square(float x, float y, float size, CRGB color, bool fill);
  void render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz);

  // debugging
  void show_pin_numbers();
};
