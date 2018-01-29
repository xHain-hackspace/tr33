// #define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// trunk config
#define TRUNK_PIN_1 27
#define TRUNK_PIN_2 25
#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 2
#define TRUNK_PIXEL_COUNT 50
#define TRUNK_STRIP_COUNT 4

// branch config
#define BRANCH_PIN_1 26
#define BRANCH_PIXEL_COUNT 300
#define BRANCH_STRIP_COUNT 1

// effect config
#define DEFAULT_SATURATION 255
#define DEFAULT_VALUE 255

// colors
#define HUE_RED 0
#define HUE_YELLOW 42
#define HUE_GREEN 85
#define HUE_BLUE  171
#define HUE_PINK 213
#define BLACK CHSV(0, 0, 0)
#define WHITE CRGB(255, 255, 255)

// effects
// #define DELAY 10
// #define SPARKLE_COUNT 3

// commands
#define COMMAND_MAX_DATA 64
#define COMMAND_BUFFER_SIZE 16

struct Command {
  uint8_t index;
  uint8_t type;
  char data[COMMAND_MAX_DATA];
};

// -- COMMANDS -----------------------------------------------------------------------
// All commands take a pointer to a buffer with its parameters.
// The comments indicate which data is expected at a certain byte index

#define DISABLE       0   // none
#define SINGLE_HUE    1   // 0: hue
#define SINGLE_COLOR  2   // 0: hue, 1: saturation 2: value
#define COLOR_WIPE    3   // 0: hue, 1: rate (pixel/s) 2: offset (pixel)
#define RAINBOW_SINE  4   // 0: rate (pixel/s) 1: wavelength (pixel) 2: width of rainbox (pixel)
#define PING_PONG     5   // 0: strip_index 1: hue, 2: rate (pixel/s) 3: width (1/10 pixel)

class Commands {
   public:
     Commands();
     void process(char* command);
     void initial();
     void run();

   private:
     void single_hue(char* data);
     void single_color(char* data);
     void color_wipe(char* data);
     void rainbow_sine(char* data);
     void ping_pong(char* data);
};
