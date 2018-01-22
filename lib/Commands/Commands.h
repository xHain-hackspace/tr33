// #define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// strip config
#define TRUNK_PIN_1 27
#define TRUNK_PIN_2 25
#define TRUNK_PIXEL_COUNT 50
#define TRUNK_STRIP_COUNT 4

#define HW_TRUNK_PIXEL_COUNT 100
#define HW_TRUNK_STRIP_COUNT 2

#define PIXEL_DENSITY 100.0

// color config
#define DEFAULT_SATURATION 255
#define DEFAULT_VALUE 255

// colors
// #define RED CHSV(0, SATURATION, VALUE)
// #define YELLOW CHSV(42, SATURATION, VALUE)
// #define GREEN CHSV(85, SATURATION, VALUE)
// #define BLUE CHSV(171, SATURATION, VALUE)
// #define PINK CHSV(213, SATURATION, VALUE)
// #define BLACK CHSV(0, 0, 0)
// #define WHITE CRGB(255, 255, 255)

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
#define RAINBOW_SINE  4   // 0: rate (pixel/s) 2: wavelength (pixel)

class Commands {
   public:
     Commands();
     void process_command(char* command);
     void initial_commands();
     void show();

   // private:
   //   void single_hue(char* data);
   //   void single_color(char * data);
   //   void color_wipe(char * data);
};
