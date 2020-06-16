#include <Arduino.h>
#include <FastLED.h>
#include <LedStructure.h>

// mapping
class Keller : public LedStructure
{
public:
  Keller();
  void write_info() { Serial.println("Keller"); }
};
