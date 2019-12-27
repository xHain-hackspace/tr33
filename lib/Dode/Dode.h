#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <Leds.h>
#endif

#define HW_STRIP_COUNT 10
#define HW_STRIP_PIXEL_COUNT 177

#define HW_STRIP_PIN_4 33
#define HW_STRIP_PIN_3 25
#define HW_STRIP_PIN_2 14
#define HW_STRIP_PIN_1 15
#define HW_STRIP_PIN_0 12

#define HW_STRIP_PIN_5 4
#define HW_STRIP_PIN_6 22
#define HW_STRIP_PIN_7 13
#define HW_STRIP_PIN_8 27
#define HW_STRIP_PIN_9 32

extern CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

#define EDGE_COUNT 30
#define CORNER_COUNT 20
#define EDGE_MAX_LENGTH 59
#define NR_LEDS_EDGE_01 59
#define NR_LEDS_EDGE_02 59
#define NR_LEDS_EDGE_03 59
#define NR_LEDS_EDGE_04 59
#define NR_LEDS_EDGE_05 59
#define NR_LEDS_EDGE_06 59
#define NR_LEDS_EDGE_07 59
#define NR_LEDS_EDGE_08 59
#define NR_LEDS_EDGE_09 59
#define NR_LEDS_EDGE_10 59
#define NR_LEDS_EDGE_11 59
#define NR_LEDS_EDGE_12 59
#define NR_LEDS_EDGE_13 59
#define NR_LEDS_EDGE_14 59
#define NR_LEDS_EDGE_15 59
#define NR_LEDS_EDGE_16 59
#define NR_LEDS_EDGE_17 59
#define NR_LEDS_EDGE_18 59
#define NR_LEDS_EDGE_19 59
#define NR_LEDS_EDGE_20 59
#define NR_LEDS_EDGE_21 59
#define NR_LEDS_EDGE_22 59
#define NR_LEDS_EDGE_23 59
#define NR_LEDS_EDGE_24 59
#define NR_LEDS_EDGE_25 59
#define NR_LEDS_EDGE_26 59
#define NR_LEDS_EDGE_27 59
#define NR_LEDS_EDGE_28 59
#define NR_LEDS_EDGE_29 59
#define NR_LEDS_EDGE_30 59
#define NR_TOTAL_LEDS (NR_LEDS_EDGE_01 + NR_LEDS_EDGE_02 + NR_LEDS_EDGE_03 + NR_LEDS_EDGE_04 + NR_LEDS_EDGE_05 + NR_LEDS_EDGE_06 + NR_LEDS_EDGE_07 + NR_LEDS_EDGE_08 + NR_LEDS_EDGE_09 + NR_LEDS_EDGE_10 + NR_LEDS_EDGE_11 + NR_LEDS_EDGE_12 + NR_LEDS_EDGE_13 + NR_LEDS_EDGE_14 + NR_LEDS_EDGE_15 + NR_LEDS_EDGE_16 + NR_LEDS_EDGE_17 + NR_LEDS_EDGE_18 + NR_LEDS_EDGE_19 + NR_LEDS_EDGE_20 + NR_LEDS_EDGE_21 + NR_LEDS_EDGE_22 + NR_LEDS_EDGE_23 + NR_LEDS_EDGE_24 + NR_LEDS_EDGE_25 + NR_LEDS_EDGE_26 + NR_LEDS_EDGE_27 + NR_LEDS_EDGE_28 + NR_LEDS_EDGE_29 + NR_LEDS_EDGE_30)
struct Coordinates
{
    uint8_t i_edge;
    uint8_t i_led;
    float x;
    float y;
    float z;
    float r;
    float phi;
    float theta;
};
extern Coordinates coordinates[NR_TOTAL_LEDS];
#define COORDINATES_MAX_R 1.40125854
#define COORDINATES_MIN_R 1.11351636

static uint16_t edges[EDGE_COUNT][3] = {
    {0, 0, NR_LEDS_EDGE_01},
    {0, NR_LEDS_EDGE_01, NR_LEDS_EDGE_02},
    {0, NR_LEDS_EDGE_01 + NR_LEDS_EDGE_02, NR_LEDS_EDGE_03},
    {1, 0, NR_LEDS_EDGE_04},
    {1, NR_LEDS_EDGE_04, NR_LEDS_EDGE_05},

    {1, NR_LEDS_EDGE_04 + NR_LEDS_EDGE_05, NR_LEDS_EDGE_06},
    {2, 0, NR_LEDS_EDGE_07},
    {2, NR_LEDS_EDGE_07, NR_LEDS_EDGE_08},
    {2, NR_LEDS_EDGE_07 + NR_LEDS_EDGE_08, NR_LEDS_EDGE_09},
    {3, 0, NR_LEDS_EDGE_10},

    {3, NR_LEDS_EDGE_10, NR_LEDS_EDGE_11},
    {3, NR_LEDS_EDGE_10 + NR_LEDS_EDGE_11, NR_LEDS_EDGE_12},
    {4, 0, NR_LEDS_EDGE_13},
    {4, NR_LEDS_EDGE_13, NR_LEDS_EDGE_14},
    {4, NR_LEDS_EDGE_13 + NR_LEDS_EDGE_14, NR_LEDS_EDGE_15},

    {5, 0, NR_LEDS_EDGE_16},
    {5, NR_LEDS_EDGE_16, NR_LEDS_EDGE_17},
    {5, NR_LEDS_EDGE_16 + NR_LEDS_EDGE_17, NR_LEDS_EDGE_18},
    {6, 0, NR_LEDS_EDGE_19},
    {6, NR_LEDS_EDGE_19, NR_LEDS_EDGE_20},

    {6, NR_LEDS_EDGE_19 + NR_LEDS_EDGE_20, NR_LEDS_EDGE_21},
    {7, 0, NR_LEDS_EDGE_22},
    {7, NR_LEDS_EDGE_22, NR_LEDS_EDGE_23},
    {7, NR_LEDS_EDGE_22 + NR_LEDS_EDGE_23, NR_LEDS_EDGE_24},
    {8, 0, NR_LEDS_EDGE_25},

    {8, NR_LEDS_EDGE_25, NR_LEDS_EDGE_26},
    {8, NR_LEDS_EDGE_25 + NR_LEDS_EDGE_26, NR_LEDS_EDGE_27},
    {9, 0, NR_LEDS_EDGE_28},
    {9, NR_LEDS_EDGE_28, NR_LEDS_EDGE_29},
    {9, NR_LEDS_EDGE_28 + NR_LEDS_EDGE_29, NR_LEDS_EDGE_30}};

struct EdgeCorner
{
    int8_t bottom_left;
    int8_t bottom_right;
    int8_t top_left;
    int8_t top_right;
};

static EdgeCorner corners[EDGE_COUNT] = {
    {5, -4, 13, 2},
    {-1, 13, 16, 3},
    {-2, 16, 20, -19},
    {8, -7, 1, 5},
    {-4, 1, 19, 6},

    {-5, 19, 23, -22},
    {11, -10, 4, 8},
    {-7, 4, 22, 9},
    {-8, 22, 26, -25},
    {25, -24, 7, 11},

    {-10, 7, 25, 12},
    {-11, 25, 29, -28},
    {2, -1, 10, 14},
    {-13, 10, 28, 15},
    {-14, 28, 17, -16},

    {3, -2, -15, 17},
    {-16, -15, -30, 18},
    {-17, -30, 21, -20},
    {6, -5, -3, 20},
    {-19, -3, -18, 21},

    {-20, 18, 24, -23},
    {9, -8, -6, 23},
    {-22, -6, 24, -21},
    {-23, -21, 27, -26},
    {12, -11, -9, 26},

    {-25, -9, -24, 27},
    {-26, -24, 30, -29},
    {15, -14, -12, 29},
    {-28, -12, -27, 30},
    {-29, -27, 18, -17}

    // {-13, 14, 4, 2},
    // {4, -1, 16, 3},
    // {16, -2, 29, -28},
    // {-1, 2, 7, 5},
    // {7, -4, 19, 6},

    // {19, -5, 17, -16},
    // {-4, 5, 10, 8},
    // {10, -7, 22, 9},
    // {22, -8, 20, -19},
    // {-7, 8, 13, 11},

    // {13, -10, 25, 12},
    // {25, -11, 23, -22},
    // {-10, 11, 1, 14},
    // {1, -13, 28, 15},
    // {28, -14, 26, -25},

    // {-2, 3, -6, 17},
    // {-6, -16, -21, 18},
    // {-21, -17, 30, -29},
    // {-5, 6, -9, 20},
    // {-9, -19, -24, 21},

    // {-24, -20, 18, -17},
    // {-8, 9, -12, 23},
    // {-12, -22, -27, 24},
    // {-27, -23, 21, -20},
    // {-11, 12, -15, 26},

    // {-15, -25, -30, 27},
    // {-30, -26, 24, -23},
    // {-14, 15, -3, 29},
    // {-3, -28, -18, 30},
    // {-18, -29, 27, -26}

};

struct Command;

class Dode : public Leds
{
public:
    Dode();
    void init();
    void set_led(uint8_t strip_index, int led, CRGB color);
    CRGB get_led(uint8_t strip_index, int led);
    void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
    uint8_t random_strip(uint8_t strip_index);
    uint16_t strip_length(uint8_t strip_index);
    uint8_t strip_count();

    void all_white();
    void kaleidoscope(char *data);
    void ping_pong(char *data);
    void random_walk(char *data);
    void debug(char *data);
    void fireworks(char *data);
    void rotating_sectors(char *data);
    void fill(char *data);
    void mapped_swipe(char *data);
    void rotating_plane(char *data);
    void twang(char *data);
    void joystick(char *data);
    static void artnet_packet_callback(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);

private:
    void set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color);
    void render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness, uint8_t ball_type);
    uint16_t edge_strip_length(Leds *leds, int8_t edge);
    void generate_mapping();
    void fill(Leds *leds, float rel_position, float width, CRGB color, float brightness, uint8_t type);
};