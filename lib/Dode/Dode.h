#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

#define HW_STRIP_COUNT 1
#define HW_STRIP_PIXEL_COUNT 120
// #define HW_STRIP_PIN_0 25
#define HW_STRIP_PIN_0 15
extern CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

#define EDGE_COUNT 30
#define EDGE_PIXEL_COUNT 4
#define CORNER_COUNT 20

static uint16_t edges[EDGE_COUNT][2] = {
    {0, 0},
    {0, 4},
    {0, 8},
    {0, 12},
    {0, 16},
    {0, 20},
    {0, 24},
    {0, 28},
    {0, 32},
    {0, 36},
    {0, 40},
    {0, 44},
    {0, 48},
    {0, 52},
    {0, 56},
    {0, 60},
    {0, 64},
    {0, 68},
    {0, 72},
    {0, 76},
    {0, 80},
    {0, 84},
    {0, 88},
    {0, 92},
    {0, 96},
    {0, 100},
    {0, 104},
    {0, 108},
    {0, 112},
    {0, 116}};

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