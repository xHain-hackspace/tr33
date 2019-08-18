#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

#define HW_STRIP_COUNT 10
#define HW_STRIP_PIXEL_COUNT 260
#define HW_STRIP_PIN_0 4
#define HW_STRIP_PIN_1 5
#define HW_STRIP_PIN_2 21
#define HW_STRIP_PIN_3 23
#define HW_STRIP_PIN_4 15

#define HW_STRIP_PIN_5 18
#define HW_STRIP_PIN_6 19
#define HW_STRIP_PIN_7 22
#define HW_STRIP_PIN_8 13
#define HW_STRIP_PIN_9 12

extern CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

#define EDGE_COUNT 30
#define CORNER_COUNT 20
#define EDGE_MAX_LENGTH 90

static uint16_t edges[EDGE_COUNT][3] = {
    {0, 0, 84},
    {0, 84, 80},
    {0, 164, 85},
    {1, 0, 84},
    {1, 84, 77},

    {1, 161, 82},
    {2, 0, 81},
    {2, 81, 78},
    {2, 159, 82},
    {3, 0, 83},

    {3, 83, 78},
    {3, 161, 83},
    {4, 0, 79},
    {4, 79, 79},
    {4, 158, 81},

    {5, 0, 84},
    {5, 84, 82},
    {5, 166, 76},
    {6, 0, 83},
    {6, 83, 84},

    {6, 167, 79},
    {7, 0, 82},
    {7, 82, 84},
    {7, 166, 78},
    {8, 0, 81},

    {8, 81, 85},
    {8, 166, 79},
    {9, 0, 81},
    {9, 81, 83},
    {9, 164, 81}};

struct EdgeCorner
{
    int8_t bottom_left;
    int8_t bottom_right;
    int8_t top_left;
    int8_t top_right;
};

static EdgeCorner corners[EDGE_COUNT] = {
    {-13, 14, 4, 2},
    {4, -1, 16, 3},
    {16, -2, 29, -28},
    {-1, 2, 7, 5},
    {7, -4, 19, 6},

    {19, -5, 17, -16},
    {-4, 5, 10, 8},
    {10, -7, 22, 9},
    {22, -8, 20, -19},
    {-7, 8, 13, 11},

    {13, -10, 25, 12},
    {25, -11, 23, -22},
    {-10, 11, 1, 14},
    {1, -13, 28, 15},
    {28, -14, 26, -25},

    {-2, 3, -6, 17},
    {-6, -16, -21, 18},
    {-21, -17, 30, -29},
    {-5, 6, -9, 20},
    {-9, -19, -24, 21},

    {-24, -20, 18, -17},
    {-8, 9, -12, 23},
    {-12, -22, -27, 24},
    {-27, -23, 21, -20},
    {-11, 12, -15, 26},

    {-15, -25, -30, 27},
    {-30, -26, 24, -23},
    {-14, 15, -3, 29},
    {-3, -28, -18, 30},
    {-18, -29, 27, -26}};

struct Command;

class Dode : public Leds
{
public:
    Dode();
    void init();
    void set_led(uint8_t strip_index, int led, CRGB color);
    void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
    uint8_t random_strip(uint8_t strip_index);
    uint16_t strip_length(uint8_t strip_index);
    uint8_t strip_count();

    void all_white();
    void kaleidoscope(char *data);
    void random_walk(char *data);
    void debug(char *data);

private:
    // Rendering
    void set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color);
};