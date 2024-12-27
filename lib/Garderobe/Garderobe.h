#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Garderobe : public LedStructure
{
public:
  Garderobe();
  String get_name() { return "Garderobe"; }
  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  void fade_led(CommandParams cmd, int led, CRGB target);
  void fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount);
  uint16_t strip_length(uint8_t strip_index);
  uint16_t pixel_count(uint8_t strip_index);
  uint8_t random_strip(uint8_t strip_index);
  uint8_t get_max_strip_index();

  std::array<int, 2> map(uint8_t strip_index, int led);
};
