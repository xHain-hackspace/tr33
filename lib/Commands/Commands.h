#define FASTLED_ESP32_FLASH_LOCK 1
#include <Arduino.h>
#include <FastLED.h>

#ifndef LEDS_H
#define LEDS_H
#include <LedStructure.h>
#endif

#define COMMAND_DATA_SIZE 10
#define COMMAND_BUFFER_SIZE 32

#define MODE_COMMANDS 0
#define MODE_STREAM 1
#define MODE_ARTNET 2

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
#define COMMAND_DEBUG 14
#define COMMAND_FIREWORKS 15
#define COMMAND_ROTATING_SECTORS 16
#define COMMAND_RENDER 17
#define COMMAND_ROTATING_PLANE 18
#define COMMAND_TWANG 19
#define COMMAND_FLICKER_SPARKLE 20

// -- EVENTS ------------------------------------------------------------------------------

#define EVENT_GRAVITY_ADD_BALL 100
#define EVENT_UPDATE_SETTINGS 101
#define EVENT_BEAT 102
#define EVENT_PIXEL 103
#define EVENT_PIXEL_RGB 104
#define EVENT_JOYSTICK 105

// -- PING_PONG ------------------------------------------------------------------------------

#define PING_PONG_LINEAR 0
#define PING_PONG_SINE 1
#define PING_PONG_NONE 2
#define PING_PONG_SAWTOOTH 3

// -- RENDER ------------------------------------------------------------------------------

#define RENDER_BALL 0
#define RENDER_COMET 1
#define RENDER_COMET_BOUNCE 2
#define RENDER_NYAN 3
#define RENDER_NYAN_BOUNCE 4
#define RENDER_FILL 5

// -- SWIPE ------------------------------------------------------------------------------

#define SWIPE_X 0
#define SWIPE_Y 1
#define SWIPE_Z 2
#define SWIPE_nX 3
#define SWIPE_nY 4
#define SWIPE_nZ 5

struct Command
{
  uint8_t index;
  uint8_t type;
  char data[COMMAND_DATA_SIZE];
};

extern Command command_buffer[COMMAND_BUFFER_SIZE];
extern CRGBPalette256 currentPalette;
extern uint8_t currentMode;

class Commands
{
private:
  LedStructure *leds;

public:
  void init(LedStructure *leds);
  void process(char *command);
  void run();
  void update_settings(char *data);

  // Commands
  void single_color(LedStructure *leds, char *data);
  void rainbow_sine(LedStructure *leds, char *data);
  void sparkle(LedStructure *leds, char *data);
  void flicker_sparkle(LedStructure *leds, char *data);
  void show_number(LedStructure *leds, char *data);
  void rain(LedStructure *leds, char *data);
  void render(LedStructure *leds, char *data);
  void ping_pong(LedStructure *leds, char *data);
  void kaleidoscope(LedStructure *leds, char *data);
  void white(LedStructure *leds, char *data);

  // Events
  void pixel(LedStructure *leds, char *data);
  void pixel_rgb(LedStructure *leds, char *data);

  // Ping Pong
  static float ping_pong_linear(uint8_t period_100ms, uint8_t offset_100ms);
  static float ping_pong_sine(uint8_t period_100ms, uint8_t offset_100ms);
  static float ping_pong_cosine(uint8_t period_100ms, uint8_t offset_100ms);
  static float ping_pong_sawtooth(uint8_t period_100ms, uint8_t offset_100ms);

  // float easing functions
  static float ease_in_cubic(float t);
  static float ease_out_cubic(float t);
  static float ease_in_out_cubic(float t);
  static uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max);

  // Render
  static void render(LedStructure *leds, uint8_t render_type, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction);
  static void render_ball(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness);
  static void render_comet(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool nyan, bool bounce);
  static void render_fill(LedStructure *leds, uint8_t strip_index, float position, CRGB color, float brightness, bool direction);

  static void artnet_sync_callback();

private:
  void render_commands();
};