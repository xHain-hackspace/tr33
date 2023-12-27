#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Minichrome : public LedStructure
{
public:
  Minichrome();
  String get_name() { return "Minichrome"; }
};
