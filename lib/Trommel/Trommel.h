#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Trommel : public LedStructure
{
public:
  Trommel();
  String get_name() { return "Trommel"; }
};
