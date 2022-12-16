#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Wolke : public LedStructure
{
public:
  Wolke();
  String get_name();
  uint8_t get_max_strip_index();

  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t pixel_count(uint8_t strip_index);
};
