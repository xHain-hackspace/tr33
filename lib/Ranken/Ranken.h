#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Ranken : public LedStructure
{
public:
  Ranken();
  String get_name() { return "Ranken"; }
};
