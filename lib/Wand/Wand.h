#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

class Wand : public LedStructure
{
public:
  Wand();
  String get_name() { return "Wand"; }
};
