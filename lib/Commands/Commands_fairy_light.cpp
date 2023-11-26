#include <Arduino.h>
#include <Commands.h>
#include <FairyLightsControl.h>
// #include "Scubar_config.h"

void Commands::fairy_light(LedStructure *leds, const CommandParams &cmd)
{
    FairyLight fairy_light = cmd.type_params.fairy_light;

    static unsigned long next_event{};
    uint8_t brightness = cmd.brightness;
    FairyPattern pattern = fairy_light.fairy_pattern;
    uint8_t frequency = fairy_light.frequency;
    uint8_t index = fairy_light.fairy_index;

    auto now = millis();

    auto fairy_light_control = leds->get_fairy_light(index);
    if (fairy_light_control == nullptr)
    {
        return;
    }

    if (pattern == FairyPattern_ODD_EVEN)
    {
        static int old_state = 0;
        // Blink odd/even
        auto state = frequency > 0 ? now / (1000 / frequency) % 2 : 0;
        if (state != old_state)
        {
            // Serial.printf("Index: %d  State: %d\n", index, state);
            old_state = state;
        }
        fairy_light_control->set_selection(state ? FairyLightsControl::LedSelection::Even : FairyLightsControl::LedSelection::Odd);
        fairy_light_control->set_brightness(brightness);
    }
    else if (pattern == FairyPattern_ODD_EVEN_ALL)
    {
        static int old_state = 0;
        // Blink odd/even/all
        auto state = frequency > 0 ? now / (1000 / frequency) % 3 : 0;
        if (state != old_state)
        {
            // Serial.printf("Index: %d  State: %d\n", index, state);
            old_state = state;
        }
        fairy_light_control->set_selection(state == 0 ? FairyLightsControl::LedSelection::Even : state == 1 ? FairyLightsControl::LedSelection::Odd
                                                                                                            : FairyLightsControl::LedSelection::All);
        fairy_light_control->set_brightness(brightness);
    }
}
