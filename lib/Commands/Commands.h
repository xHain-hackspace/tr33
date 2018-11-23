#include <Arduino.h>
#include <FastLED.h>

// trunk config
#define TRUNK_PIN_1 5
#define TRUNK_PIN_2 4
#define TRUNK_PIN_3 33
#define TRUNK_PIN_4 27
#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 4
#define TRUNK_PIXEL_COUNT HW_TRUNK_PIXEL_COUNT / 2
#define TRUNK_STRIP_COUNT HW_TRUNK_STRIP_COUNT * 2

// branch config
#define BRANCH_PIN_1 23
#define BRANCH_PIN_2 21
#define BRANCH_PIN_3 19
#define BRANCH_PIN_4 18
#define BRANCH_PIN_5 22
#define BRANCH_PIN_6 14
#define BRANCH_PIN_7 32
#define BRANCH_PIN_8 15
#define BRANCH_PIXEL_COUNT 90
#define BRANCH_STRIP_COUNT 8
#define BRANCH_OFFSET 50

// strip index
#define STRIP_INDEX_ALL_TRUNKS TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT
#define STRIP_INDEX_ALL_BRANCHES TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 1
#define STRIP_INDEX_ALL TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 2
#define STRIP_INDEX_SPIRAL TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT + 3

// colors
#define HUE_RED 0
#define HUE_YELLOW 42
#define HUE_GREEN 85
#define HUE_BLUE  171
#define HUE_PINK 213
#define COLOR_BLACK CHSV(0, 0, 0)
#define COLOR_WHITE CRGB(255, 255, 255)

// commands
#define COMMAND_MAX_DATA 8
#define COMMAND_BUFFER_SIZE 16

// effect config
#define DEFAULT_PALETTE Rainbow_gp

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70
#define MAX_SPARKLES 500
#define SPARKLES_DIM_RATE 40

#include <Overrides.h>

struct Command {
  uint8_t index;
  uint8_t type;
  char data[COMMAND_MAX_DATA];
};

// -- COMMANDS ---------------------------------------------------------------------------

#define DISABLE             0   
#define SINGLE_COLOR        1   
#define WHITE               2
#define RAINBOW_SINE        3   
#define PING_PONG           4   
#define GRAVITY             5   
#define SPARKLE             6
#define SHOW_NUMBER         7

// -- EVENTS ------------------------------------------------------------------------------

#define GRAVITY_ADD_BALL    100
#define UPDATE_SETTINGS     101

// -- BALL_TYPES --------------------------------------------------------------------------

#define BALL_TYPE_SQUARE        0
#define BALL_TYPE_SINE          1 
#define BALL_TYPE_COMET         2
#define BALL_TYPE_FILL_TOP      3
#define BALL_TYPE_FILL_BOTTOM   4

extern CRGBPalette256 currentPalette;

class Commands {
   public:
     Commands();
     void init();
     void process(char* command);
     void run();

   private:
     // commands
     void single_color(char* data);
     void rainbow_sine(char* data);
     void ping_pong(char* data);
     void gravity(char* data);
     void sparkle(char* data);
     void show_number(char* data);
    //  void spiral(char* data);

    // one-off events
    void gravity_event();
    void update_settings(char * data);

    // set leds
    void all_off();
    void all_white();
    void set_led(uint8_t strip_index, int led, CRGB color);
    void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
    int strip_index_length(uint8_t strip_index);

    // ball rendering
    void render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top);
    void render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
    void render_sine_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
    void render_comet(uint8_t strip_index, float center, float length, CRGB color, bool bounce_top);
    void render_fill_top(uint8_t strip_index, float center, CRGB color);
    void render_fill_bottom(uint8_t strip_index, float center, CRGB color);

    // debugging
    void show_pin_numbers();

    // helper 
    uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max);
    uint8_t random_strip(uint8_t strip_index);
    

};
