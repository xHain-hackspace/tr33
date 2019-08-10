#include <Arduino.h>
#include <FastLED.h>

#define COMMAND_DATA_SIZE 8
#define COMMAND_BUFFER_SIZE 16

#define MODE_COMMANDS 0
#define MODE_STREAM 1

// colors
#define HUE_RED 0
#define HUE_YELLOW 42
#define HUE_GREEN 85
#define HUE_BLUE 171
#define HUE_PINK 213
#define COLOR_BLACK CHSV(0, 0, 0)
#define COLOR_WHITE CRGB(255, 255, 255)

// -- COMMANDS ---------------------------------------------------------------------------

#define COMMAND_DISABLE 0
#define COMMAND_SINGLE_COLOR 1
#define COMMAND_WHITE 2
#define COMMAND_RAINBOW_SINE 3
#define COMMAND_PING_PONG 4
#define COMMAND_GRAVITY 5
#define COMMAND_SPARKLE 6
#define COMMAND_SHOW_NUMBER 7
#define COMMAND_RAIN 8
#define COMMAND_BEATS 9
#define COMMAND_MAPPED_SWIPE 10
#define COMMAND_MAPPED_SHAPE 11
#define COMMAND_KALEIDOSCOPE 12
#define COMMAND_RANDOM_WALK 13

// -- EVENTS ------------------------------------------------------------------------------

#define EVENT_GRAVITY_ADD_BALL 100
#define EVENT_UPDATE_SETTINGS 101
#define EVENT_BEAT 102
#define EVENT_PIXEL 103
#define EVENT_PIXEL_RGB 104

struct Command
{
  uint8_t index;
  uint8_t type;
  char data[COMMAND_DATA_SIZE];
};

extern Command command_buffer[COMMAND_BUFFER_SIZE];
extern CRGBPalette256 currentPalette;
extern uint8_t currentMode;

struct Leds;

class Commands
{
public:
  void init();
  void process(char *command);
  void run();
  void update_settings(char *data);

  // Commands
  void sparkle(Leds *leds, char *data);

  // Helpers
  static float ping_pong_linear(uint8_t period_100ms, uint8_t offset_100ms);

  // float easing functions
  static float ease_in_cubic(float t);
  static float ease_out_cubic(float t);
  static float ease_in_out_cubic(float t);
  static uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max);

  static void render_ball(Leds *leds, uint8_t strip_index, float center, float width, CRGB color, float ball_brightness);
};