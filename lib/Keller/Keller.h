#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

// strip config
#define STRIP_PIN_1 33
#define STRIP_PIN_2 25
#define STRIP_PIN_3 14
#define STRIP_PIN_4 15
#define STRIP_PIN_5 12
#define STRIP_PIN_6 4
#define STRIP_PIN_7 22
#define STRIP_PIN_8 13
#define STRIP_PIN_9 27
#define STRIP_PIN_10 32
#define STRIP_PIXEL_COUNT 249
#define STRIP_COUNT 10

// strip index
#define STRIP_INDEX_ALL STRIP_COUNT

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

class Keller : public Leds
{

public:
  Keller();
  void init();
  void set_led(uint8_t strip_index, int led, CRGB color);
  void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint8_t strip_count();

  // commands - rendered on each loop
  void all_white();
  void rainbow_sine(char *data);
  void ping_pong(char *data);
  void fill(char *data);

  // void gravity(char *data);
  // void rain(char *data);
  // void beats(char *data);
  // void mapped_swipe(char *data);
  // void mapped_shape(char *data);
  // void twang(char *data);

  // // events - rendered once
  // void gravity_event();
  // void beat(char *data);
  // void joystick(char *data);

private:
  // ball rendering
  void render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness, uint8_t ball_type);
  void fill(Leds *leds, uint8_t strip_index, float rel_position, float width, CRGB color, float brightness, uint8_t type);

  // // mapped rendering
  // void render_mapped_square(float x, float y, float size, CRGB color, bool fill);
  // void render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz);

  // // debugging
  // void show_pin_numbers();
};
