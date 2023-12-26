#ifndef __FAIRYLIGHTSCONTROL_H_INCLUDED__
#define __FAIRYLIGHTSCONTROL_H_INCLUDED__

#pragma once
#include <Arduino.h>
#include <cstdint>

/**
 * Controller for fairy lights
 *
 * This is intended to control fairy lights that are connected in parallel with two wires with
 * alternating cathode and anode in such a way that applying (+VCC GND) to the wires lights half of the lights
 * and connecting (GND +VCC) lights the other half of the lights.
 *
 * This type of arrangement can be controlled using an Half-bridge such as the L293/L293D with the SELECT pin connected
 * to one input, and via a NOT-gate to the second input, while the PWM pin is connected to ENABLE.
 */
class FairyLightsControl
{
public:
    /**
     * Construct fairy lights control
     *
     * @note Some LEDC channels seem to use the same timer which means that their timing cannot be different.
     *       You should ensure that the PWM channel and the SELECT channel are NOT using the same timer.
     *       If you notice that you are only able to turn on half of the lights, this is usually the reason.
     *
     * @param pwm_channel LEDC channel used for PWM
     * @param pwm_pin GPIO pin for PWM
     * @param select_channel LEDC channel used for selecting between odd/even LEDs
     * @param select_pin GPIO used for selecting between odd/even LEDs
     */
    FairyLightsControl(uint8_t pwm_channel, uint8_t pwm_pin, uint8_t select_channel, uint8_t select_pin);

    enum class LedSelection
    {
        Odd,  //!< Turn on every odd light
        Even, //!< Turn on every even light
        All   //!< Turn on all lights
    };

    /**
     * @param selection Which lights to turn on
     */
    void set_selection(LedSelection selection) const;

    /**
     * @param brightness Brightness of the light 1-255
     */
    void set_brightness(uint8_t brightness) const;

private:
    uint8_t pwm_channel_;
    uint8_t select_channel_;
};

#endif // __FAIRYLIGHTSCONTROL_H_INCLUDED__