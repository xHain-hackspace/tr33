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
};
