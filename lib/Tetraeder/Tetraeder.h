#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

#define EDGE_COUNT 24
#define EDGE_LENGTH 59
#define CORNER_COUNT 10

class Tetraeder : public LedStructure
{
public:
  Tetraeder();
  String get_name() { return "Tetraeder"; }

  // set leds
  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  uint16_t strip_length(uint8_t strip_index);
  uint8_t random_strip(uint8_t strip_index);
};
