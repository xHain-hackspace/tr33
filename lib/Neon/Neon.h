#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

class Neon : public LedStructure
{
public:
  Neon();
  String get_name() { return "Neon"; }
};
