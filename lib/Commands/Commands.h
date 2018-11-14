#include <Arduino.h>
#include <FastLED.h>

// trunk config
#define TRUNK_PIN_1 25
#define TRUNK_PIN_2 26
#define TRUNK_PIN_3 14
#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 3
#define TRUNK_PIXEL_COUNT 50
#define TRUNK_STRIP_COUNT 6

// branch config
#define BRANCH_PIN_1 4
#define BRANCH_PIN_2 12
#define BRANCH_PIN_3 13
#define BRANCH_PIN_4 15
#define BRANCH_PIN_5 21
#define BRANCH_PIN_6 22
#define BRANCH_PIXEL_COUNT 90
#define BRANCH_STRIP_COUNT 6
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
#define DEFAULT_PALETTE RainbowColors_p
#define DEFAULT_SATURATION 255
#define DEFAULT_VALUE 255

#define MAX_GRAVITY_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70
// #define GRAVITY_VALUE 18
// #define GRAVITY_DAMPING 100
#define MAX_SPARKLES 500
#define SPARKLES_DIM_RATE 40

#define BALL_TYPE_SQUARE 0
#define BALL_TYPE_SINE   1 
#define BALL_TYPE_COMET  2

#include <Overrides.h>

struct Command {
  uint8_t index;
  uint8_t type;
  char data[COMMAND_MAX_DATA];
};

// -- COMMANDS -----------------------------------------------------------------------
// All commands take a pointer to a buffer with its parameters.
// The comments indicate which data is expected at a certain byte index

#define DISABLE             0   
#define SINGLE_HUE          1   
#define SINGLE_COLOR        2   
#define COLOR_WIPE          3   
#define RAINBOW_SINE        4   
#define PING_PONG           5   
#define GRAVITY             6   
#define OFF                 7   
#define WHITE               8   
#define SPARKLE             9   
#define SPIRAL             10    

// -- EVENTS --------------------------------------------------------------------------

#define GRAVITY_EVENT       100

extern CRGBPalette256 currentPalette;

class Commands {
   public:
     Commands();
     void init();
     void process(char* command);
     void run();

   private:
     // commands
     void single_hue(char* data);
     void single_color(char* data);
     void color_wipe(char* data);
     void rainbow_sine(char* data);
     void ping_pong(char* data);
     void gravity(char* data);
     void off(char* data);
     void white(char* data);
     void sparkle(char* data);
    //  void spiral(char* data);

    // events
    void gravity_event();

    // set leds
    void set_led(int strip_index, int led, CRGB color);
    void fade_led(int strip_index, int led, CRGB target, float intensity);
    int strip_index_length(int strip_index);

    // ball rendering
    void render_ball(int strip_index, int ball_type, float center, float width, CHSV color, float ball_intensity, bool bounce_top);
    void render_square_ball(int strip_index, float center, float width, CHSV color, float ball_intensity);
    void render_sine_ball(int strip_index, float center, float width, CHSV color, float ball_intensity);
    void render_tail(int strip_index, float center, float length, CHSV color, bool bounce_top);};
