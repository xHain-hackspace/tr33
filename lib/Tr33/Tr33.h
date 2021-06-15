#include <Arduino.h>
#include <FastLED.h>
#include <LedStructure.h>
#include <command_schemas.pb.h>
#include <Tr33_config.h>

class Tr33 : public LedStructure
{

public:
  Tr33();

  static CRGB trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
  static CRGB branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  void fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount);
  void fade_single_led(uint8_t strip_index, int led, CRGB target, fract8 amount);
  void fade_all_trunks(int led, CRGB target, fract8 amount);
  void fade_all_branches(int led, CRGB target, fract8 amount);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint16_t pixel_count(uint8_t strip_index);

  void write_info() { Serial.println("Tr33"); }
};
