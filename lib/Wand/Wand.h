#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Wand : public LedStructure
{
public:
  Wand();
  void write_info() { Serial.println("Wand"); }
};
