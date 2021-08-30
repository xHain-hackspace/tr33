#include <Arduino.h>
#include <FastLED.h>
#include <LedStructure.h>
#include <FairyLightsControl.h>

class Scubar : public LedStructure
{
public:
    Scubar();
    void write_info() override { Serial.println("Scubar"); }
    void init() override;

private:
    FairyLightsControl* get_fairy_light(uint8_t index) override;
    std::array<FairyLightsControl, FAIRY_LIGHTS_COUNT> fairy_lights_;
};
