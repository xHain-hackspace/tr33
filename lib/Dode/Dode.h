#include <Arduino.h>
#include <FastLED.h>

#define HW_STRIP_COUNT 1
#define HW_STRIP_PIXEL_COUNT 120
#define HW_STRIP_PIN_0 27
extern CRGB leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

#define EDGE_COUNT 30
#define EDGE_PIXEL_COUNT 4

// COMMANDS

#define COMMAND_RANDOM_WALK 12

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

struct Command;

class Dode
{
public:
    Dode();
    void init();
    void all_off();
    void process_event(Command *command);
    void process_command(Command *command);

private:
    // Commands
    void single_color(char *data);
    void random_walk(char *data);

    // Rendering
    void set_led(uint8_t edge_index, uint8_t led, CRGB color);
};