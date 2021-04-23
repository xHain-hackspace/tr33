#include "FairyLightsControl.h"


FairyLightsControl::FairyLightsControl(uint8_t pwm_channel, uint8_t pwm_pin, uint8_t select_channel, uint8_t select_pin)
    : pwm_channel_(pwm_channel)
    , select_channel_(select_channel)
{
    ledcSetup(select_channel, 100, 8);
    ledcSetup(pwm_channel, 2000, 8);

    // Attach pints to LEDC channels
    ledcAttachPin(pwm_pin, pwm_channel);
    ledcAttachPin(select_pin, select_channel);

    // Default values
    set_selection(LedSelection::All);
    set_brightness(255);
}

void FairyLightsControl::set_selection(LedSelection selection) const {
    switch (selection) {
        case LedSelection::Odd:
            ledcWrite(select_channel_, 255);
            break;
        case LedSelection::Even:
            ledcWrite(select_channel_, 0);
            break;
        case LedSelection::All:
            ledcWrite(select_channel_, 127);
            break;
    }
}

void FairyLightsControl::set_brightness(uint8_t brightness) const {
    ledcWrite(pwm_channel_, brightness);
}
