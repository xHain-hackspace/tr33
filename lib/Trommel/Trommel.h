#include <Arduino.h>
#include <FastLED.h>
#include <LedStructure.h>

// mapping
class Trommel : public LedStructure
{
public:
  Trommel();
  void write_info() { Serial.println("Trommel"); }
};
