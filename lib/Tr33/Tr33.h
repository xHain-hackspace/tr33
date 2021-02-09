#include <Arduino.h>
#include <FastLED.h>
#include <LedStructure.h>
#include <Tr33_config.h>

class Tr33 : public LedStructure
{

public:
  Tr33();

  static CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
  static CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint16_t pixel_count(uint8_t strip_index);
  // uint8_t strip_count();

  void write_info() { Serial.println("Tr33"); }

  // commands - rendered on each loop
  // void gravity(uint8_t *data);
  // void beats(uint8_t *data);
  // void mapped_swipe(uint8_t *data);
  // void mapped_shape(uint8_t *data);
  // void twang(uint8_t *data);

  // events - rendered once
  // void gravity_event();
  // void beat(uint8_t *data);
  // void joystick(uint8_t *data);

private:
  // // // ball rendering
  // void render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  // void render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
  // void render_comet(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  // void render_nyan(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom);
  // void render_fill_top(uint8_t strip_index, float center, CRGB color, float ball_brightness);
  // void render_fill_bottom(uint8_t strip_index, float center, CRGB color, float ball_brightness);

  // mapped rendering
  // void render_mapped_square(float x, float y, float size, CRGB color, bool fill);
  // void render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz);

  // debugging
  // void show_pin_numbers();
};
