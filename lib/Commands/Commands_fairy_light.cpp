#include <Arduino.h>
#include <Commands.h>
#include <FairyLightsControl.h>
// #include "Scubar_config.h"

/*
static FairyLightsControl control1(0, FAIRY_S1_PWM, 2, FAIRY_S1_SEL);
static FairyLightsControl control2(0, FAIRY_S2_PWM, 2, FAIRY_S2_SEL);
static FairyLightsControl control3(0, FAIRY_S3_PWM, 2, FAIRY_S3_SEL);
static FairyLightsControl control4(0, FAIRY_S4_PWM, 2, FAIRY_S4_SEL);

static unsigned long next = 0;
#include <array>

void Commands::fairy_light(LedStructure *leds, uint8_t *data) {
    auto now = millis();
    if (now > next) {
        static auto mode = FairyLightsControl::LedSelection::Even;
        next = now+1000;

        switch (mode) {
            case FairyLightsControl::LedSelection::Even:
                mode = FairyLightsControl::LedSelection::Odd;
                digitalWrite(27, HIGH);
                break;
            case FairyLightsControl::LedSelection::Odd:
                mode = FairyLightsControl::LedSelection::All;
                digitalWrite(27, LOW);
                break;
            case FairyLightsControl::LedSelection::All:
                mode = FairyLightsControl::LedSelection::Even;
                break;
        }

        Serial.printf("Switching mode to %d\n", mode);
        control1.set_selection(mode);
        control2.set_selection(mode);
        control3.set_selection(mode);
        control4.set_selection(mode);
    }

//    auto ratio = (sin(now / 100.0) + 1) / 2;
    //auto value = static_cast<int>(ratio*254)+1;
    control1.set_brightness(127);
    control2.set_brightness(127);
    control3.set_brightness(127);
    control4.set_brightness(127);
    //Serial.printf("Ratio %.2f value %d\n", ratio, value);

    //control.set_selection(now / 500 % 2 == 0 ? FairyLightsControl::LedSelection::Odd : FairyLightsControl::LedSelection::Even);
    //control.set_selection(FairyLightsControl::LedSelection::All);
}
*/

void Commands::fairy_light(LedStructure *leds, CommandParams cmd)
{
    FairyLight fairy_light = cmd.type_params.fairy_light;

    static unsigned long next_event{};
    uint8_t index = cmd.strip_index;
    uint8_t brightness = cmd.brightness;
    uint8_t pattern = fairy_light.pattern;
    uint8_t frequency = fairy_light.frequency;

    auto now = millis();

    auto fairy_light_control = leds->get_fairy_light(index);
    if (fairy_light_control == nullptr)
    {
        return;
    }

    if (pattern == 0)
    {
        static int old_state = 0;
        // Blink odd/even
        auto state = frequency > 0 ? now / (1000 / frequency) % 2 : 0;
        if (state != old_state)
        {
            Serial.printf("Index: %d  State: %d\n", index, state);
            old_state = state;
        }
        fairy_light_control->set_selection(state ? FairyLightsControl::LedSelection::Even : FairyLightsControl::LedSelection::Odd);
        fairy_light_control->set_brightness(brightness);
    }
    else if (pattern == 1)
    {
        static int old_state = 0;
        // Blink odd/even/all
        auto state = frequency > 0 ? now / (1000 / frequency) % 3 : 0;
        if (state != old_state)
        {
            Serial.printf("Index: %d  State: %d\n", index, state);
            old_state = state;
        }
        fairy_light_control->set_selection(state == 0 ? FairyLightsControl::LedSelection::Even : state == 1 ? FairyLightsControl::LedSelection::Odd
                                                                                                            : FairyLightsControl::LedSelection::All);
        fairy_light_control->set_brightness(brightness);
    }
}
