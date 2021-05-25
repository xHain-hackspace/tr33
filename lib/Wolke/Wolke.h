#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

// mapping
class Wolke : public LedStructure
{
public:
  Wolke();
  void write_info() { Serial.println("Wolke"); }
};
