#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Arrow : public LedStructure
{
public:
  Arrow();
  String get_name() { return "Arrow"; }
};
