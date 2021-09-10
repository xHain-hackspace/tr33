#ifndef BEATS_H /* include guards */
#define BEATS_H

// hardware pin definitions - change to suit circuit
#define MSGEQ7_DATA_PIN 34
#define MSGEQ7_RESET_PIN 15
#define MSGEQ7_STROBE_PIN 13

// frequency reading the IC data
#define MSGEQ7_READ_DELAY 15

class Beats
{
public:
    static uint16_t read_band(uint8_t index);
};

#endif /* BEATS_H */
