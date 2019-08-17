#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

#define HW_STRIP_COUNT 1
#define HW_STRIP_PIXEL_COUNT 120
#define HW_STRIP_PIN_0 33
#define HW_STRIP_PIN_1 13
#define HW_STRIP_PIN_2 14
#define HW_STRIP_PIN_3 27
#define HW_STRIP_PIN_4 25
#define HW_STRIP_PIN_5 26
#define HW_STRIP_PIN_6 32
#define HW_STRIP_PIN_7 15
#define HW_STRIP_PIN_8 4
#define HW_STRIP_PIN_9 15

extern CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

#define EDGE_COUNT 30
#define EDGE_PIXEL_COUNT 74
#define CORNER_COUNT 20

static uint16_t edges[EDGE_COUNT][2] = {
    {0, 0},
    {0, 74},
    {0, 148},
    {1, 0},
    {1, 74},
    {1, 148},
    {2, 0},
    {2, 74},
    {2, 148},
    {3, 0},
    {3, 74},
    {3, 148},
    {4, 0},
    {4, 74},
    {4, 148},
    {5, 0},
    {5, 74},
    {5, 148},
    {6, 0},
    {6, 74},
    {6, 148},
    {7, 0},
    {7, 74},
    {7, 148},
    {8, 0},
    {8, 74},
    {8, 148},
    {9, 0},
    {9, 74},
    {9, 148}};

struct EdgeCorner
{
    int8_t bottom_left;
    int8_t bottom_right;
    int8_t top_left;
    int8_t top_right;
};

static EdgeCorner corners[EDGE_COUNT] = {
    {10, -9, 2, 3},
    {-1, 3, -13, 14},
    {2, -1, 4, 5},
    {-3, 5, -15, 16},
    {4, -3, 6, 7},
    {-5, 7, -17, 18},
    {6, -5, 8, 9},
    {-7, 9, -19, 20},
    {8, -7, 10, 1},
    {-9, 1, -11, 12},
    {-30, -20, 12, -10},
    {-11, -10, -22, 13},
    {-22, -12, 14, -2},
    {-13, -2, -24, 15},
    {-24, -14, 16, -4},
    {-15, -4, -26, 17},
    {-26, -16, 18, -6},
    {-17, -6, -28, 19},
    {-28, -18, 20, -8},
    {-19, -8, -30, 11},
    {-29, 30, 23, 22},
    {23, -21, 13, -12},
    {22, -21, 25, 24},
    {-23, 25, 15, -14},
    {24, -23, 27, 26},
    {-25, 27, 17, -16},
    {26, -25, 29, 28},
    {-27, 29, 19, -18},
    {28, -27, 21, 30},
    {-29, 21, 11, -20}};

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

private:
    // Rendering
    void set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color);
};