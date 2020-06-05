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
#define STRIP_PIXEL_COUNT 300
#define STRIP_COUNT 10

// strip index
#define STRIP_INDEX_ALL STRIP_COUNT

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70
#define MAX_RAIN_DROPS 500

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
  CRGB get_led(uint8_t strip_index, int led);
  void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint8_t strip_count();

  // commands - rendered on each loop
  void all_white();

  // void gravity(char *data);
  // void mapped_swipe(char *data);
  // void mapped_shape(char *data);
  // void twang(char *data);

  // // events - rendered once
  // void gravity_event();
  // void beat(char *data);
  // void joystick(char *data);

private:
  // // mapped rendering
  // void render_mapped_square(float x, float y, float size, CRGB color, bool fill);
  // void render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz);

  // // debugging
  // void show_pin_numbers();
};
