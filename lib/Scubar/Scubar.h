#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>
#include <FairyLightsControl.h>
#include <Scubar_config.h>

class Scubar : public LedStructure
{
public:
    Scubar();
    String get_name() { return "Scubar"; }
    void init() override;

private:
    FairyLightsControl *get_fairy_light(uint8_t index) override;
    std::array<FairyLightsControl, FAIRY_LIGHTS_COUNT> fairy_lights_;
};
