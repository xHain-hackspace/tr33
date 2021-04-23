#include <array>
#include "Commands.h"
#include "Scubar.h"

Scubar::Scubar() :
    fairy_lights_({
        FairyLightsControl{0, FAIRY_S1_PWM, 4, FAIRY_S1_SEL},
        FairyLightsControl{1, FAIRY_S2_PWM, 5, FAIRY_S2_SEL},
        FairyLightsControl{2, FAIRY_S3_PWM, 6, FAIRY_S3_SEL},
        FairyLightsControl{3, FAIRY_S4_PWM, 7, FAIRY_S4_SEL}
    })
{
    FastLED.addLeds<WS2812B, 5>(leds[0], 8*8);
}

void Scubar::init() {
    command_buffer[0].type = COMMAND_RAINBOW_SINE;
    command_buffer[0].data[0] = 0;  // strip index
    command_buffer[0].data[1] = 60;  // rate
    command_buffer[0].data[2] = 80;  // wavelength
    command_buffer[0].data[3] = 80;  // width_percent
    command_buffer[0].data[4] = 5;  // max brightness

    command_buffer[1].type = COMMAND_FAIRY_LIGHT;
    command_buffer[1].data[0] = 0;  // Index
    command_buffer[1].data[1] = 0;  // Pattern
    command_buffer[1].data[2] = 8;  // Frequency
    command_buffer[1].data[3] = 32;  // Brightness

    command_buffer[2].type = COMMAND_FAIRY_LIGHT;
    command_buffer[2].data[0] = 1;  // Index
    command_buffer[2].data[1] = 0;  // Pattern
    command_buffer[2].data[2] = 4;  // Frequency
    command_buffer[2].data[3] = 64;  // Brightness

    command_buffer[3].type = COMMAND_FAIRY_LIGHT;
    command_buffer[3].data[0] = 2;  // Index
    command_buffer[3].data[1] = 0;  // Pattern
    command_buffer[3].data[2] = 2;  // Frequency
    command_buffer[3].data[3] = 127;  // Brightness

    command_buffer[4].type = COMMAND_FAIRY_LIGHT;
    command_buffer[4].data[0] = 3;  // Index
    command_buffer[4].data[1] = 0;  // Pattern
    command_buffer[4].data[2] = 1;  // Frequency
    command_buffer[4].data[3] = 255;  // Brightness
}

FairyLightsControl* Scubar::get_fairy_light(uint8_t index) {
    if (index < fairy_lights_.size()) {
        return &fairy_lights_[index];
    }

    return nullptr;
}
