#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Discotestrig : public LedStructure
{
public:
  Discotestrig();
  void write_info() { Serial.println("Discotestrig"); }
};
