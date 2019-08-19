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
#define NR_LEDS_EDGE_01 84 
#define NR_LEDS_EDGE_02 80 
#define NR_LEDS_EDGE_03 85 
#define NR_LEDS_EDGE_04 84
#define NR_LEDS_EDGE_05 77
#define NR_LEDS_EDGE_06 82
#define NR_LEDS_EDGE_07 81
#define NR_LEDS_EDGE_08 78
#define NR_LEDS_EDGE_09 82
#define NR_LEDS_EDGE_10 83   
#define NR_LEDS_EDGE_11 78
#define NR_LEDS_EDGE_12 83
#define NR_LEDS_EDGE_13 79
#define NR_LEDS_EDGE_14 79
#define NR_LEDS_EDGE_15 81
#define NR_LEDS_EDGE_16 84
#define NR_LEDS_EDGE_17 82
#define NR_LEDS_EDGE_18 76
#define NR_LEDS_EDGE_19 83
#define NR_LEDS_EDGE_20 84
#define NR_LEDS_EDGE_21 79
#define NR_LEDS_EDGE_22 82
#define NR_LEDS_EDGE_23 84
#define NR_LEDS_EDGE_24 78
#define NR_LEDS_EDGE_25 81
#define NR_LEDS_EDGE_26 85
#define NR_LEDS_EDGE_27 79
#define NR_LEDS_EDGE_28 81
#define NR_LEDS_EDGE_29 83
#define NR_LEDS_EDGE_30 81
#define NR_TOTAL_LEDS ( NR_LEDS_EDGE_01 +NR_LEDS_EDGE_02 +NR_LEDS_EDGE_03 +NR_LEDS_EDGE_04 +NR_LEDS_EDGE_05 +NR_LEDS_EDGE_06 +NR_LEDS_EDGE_07 +NR_LEDS_EDGE_08 +NR_LEDS_EDGE_09 +NR_LEDS_EDGE_10 +NR_LEDS_EDGE_11 +NR_LEDS_EDGE_12 +NR_LEDS_EDGE_13 +NR_LEDS_EDGE_14 +NR_LEDS_EDGE_15 +NR_LEDS_EDGE_16 +NR_LEDS_EDGE_17 +NR_LEDS_EDGE_18 +NR_LEDS_EDGE_19 +NR_LEDS_EDGE_20 +NR_LEDS_EDGE_21 +NR_LEDS_EDGE_22 +NR_LEDS_EDGE_23 +NR_LEDS_EDGE_24 +NR_LEDS_EDGE_25 +NR_LEDS_EDGE_26 +NR_LEDS_EDGE_27 +NR_LEDS_EDGE_28 +NR_LEDS_EDGE_29 +NR_LEDS_EDGE_30 )
extern float LEDs_cartesian[NR_TOTAL_LEDS][5];// Kartesische Koordinaten:  {Kantenindex, LED-Index, X, Y, Z}
extern float LEDs_spherical[NR_TOTAL_LEDS][5];// Kugel-Koordinaten:  {Kantenindex, LED-Index, r, phi, theta}
struct Coordinates;
extern Coordinates coordinates[NR_TOTAL_LEDS];

static uint16_t edges[EDGE_COUNT][3] = {
    {0, 0, NR_LEDS_EDGE_01},
    {0, 84, NR_LEDS_EDGE_02},
    {0, 164, NR_LEDS_EDGE_03},
    {1, 0, NR_LEDS_EDGE_04},
    {1, 84, NR_LEDS_EDGE_05},

    {1, 161, NR_LEDS_EDGE_06},
    {2, 0, NR_LEDS_EDGE_07},
    {2, 81, NR_LEDS_EDGE_08},
    {2, 159, NR_LEDS_EDGE_09},
    {3, 0, NR_LEDS_EDGE_10},

    {3, 83, NR_LEDS_EDGE_11},
    {3, 161, NR_LEDS_EDGE_12},
    {4, 0, NR_LEDS_EDGE_13},
    {4, 79, NR_LEDS_EDGE_14},
    {4, 158, NR_LEDS_EDGE_15},

    {5, 0, NR_LEDS_EDGE_16},
    {5, 84, NR_LEDS_EDGE_17},
    {5, 166, NR_LEDS_EDGE_18},
    {6, 0, NR_LEDS_EDGE_19},
    {6, 83, NR_LEDS_EDGE_20},

    {6, 167, NR_LEDS_EDGE_21},
    {7, 0, NR_LEDS_EDGE_22},
    {7, 82, NR_LEDS_EDGE_23},
    {7, 166, NR_LEDS_EDGE_24},
    {8, 0, NR_LEDS_EDGE_25},

    {8, 81, NR_LEDS_EDGE_26},
    {8, 166, NR_LEDS_EDGE_27},
    {9, 0, NR_LEDS_EDGE_28},
    {9, 81, NR_LEDS_EDGE_29},
    {9, 164, NR_LEDS_EDGE_30}};

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
    void fireworks(char *data);

private:
    // Rendering
    void set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color);

    void render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness, uint8_t ball_type);
    uint16_t edge_strip_length(Leds *leds, int8_t edge);

    void generate_mapping();
};