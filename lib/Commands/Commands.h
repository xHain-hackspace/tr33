#ifndef __COMMANDS_H_INCLUDED__
#define __COMMANDS_H_INCLUDED__

#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <LedStructure.h>

#define COMMAND_COUNT 8
#define COMMAND_HASH_SIZE 4

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

// -- EVENTS ------------------------------------------------------------------------------

#define EVENT_GRAVITY_ADD_BALL 100
#define EVENT_UPDATE_SETTINGS 101
#define EVENT_BEAT 102
#define EVENT_PIXEL 103
#define EVENT_PIXEL_RGB 104
#define EVENT_TWANG_JOYSTICK 105
#define EVENT_MODIFIER_UPDATE 106

// -- SWIPE ------------------------------------------------------------------------------

#define SWIPE_X 0
#define SWIPE_Y 1
#define SWIPE_Z 2
#define SWIPE_nX 3
#define SWIPE_nY 4
#define SWIPE_nZ 5

extern CommandParams commands[COMMAND_COUNT];

#define EFFECT_ITEM_COUNT 1200
struct EffectItem
{
  bool enabled;
  uint8_t type;
  uint8_t strip_index;
  uint8_t command_index;
  uint8_t param_1;
  CRGB color;
  float center;
  uint32_t start_time;
};
extern EffectItem effect_items[EFFECT_ITEM_COUNT];

class Commands
{
private:
  static int64_t millis_offset;
  uint16_t effect_item_index = 0;
  bool send_debug = false;

public:
  LedStructure *leds;
  static uint8_t joystick_x;
  static uint8_t joystick_y;
  static bool joystick_button;
  static FirmwareConfig firmware_config;

  void init(LedStructure *leds);
  void process(uint8_t *command, int bytes);
  void handle_message(WireMessage msg);
  void handle_command(CommandParams cmd);
  void run();

  // Misc
  void update_settings(uint8_t *data);
  static void load_palettes();
  static CRGB color_from_palette(CommandParams cmd, uint8_t color);
  static CRGB color_from_palette(CommandParams cmd, uint8_t color, uint8_t brightness);
  static CRGB color_from_palette(ColorPalette palette_tag, uint8_t color_index, uint8_t brightness);
  static uint64_t synced_millis();
  void write_hashes(TargetMetrics *target_metrics);

  // Commands
  void white(LedStructure *leds, CommandParams cmd);
  void single_color(LedStructure *leds, CommandParams cmd);
  void calibrate(LedStructure *leds, CommandParams cmd);
  void pixel(LedStructure *leds, CommandParams cmd);
  void pixel_rgb(LedStructure *leds, CommandParams cmd);
  void rainbow(LedStructure *leds, CommandParams cmd);
  void sparkle(LedStructure *leds, CommandParams cmd);
  void flicker_sparkle(LedStructure *leds, CommandParams cmd);
  void show_number(LedStructure *leds, CommandParams cmd);
  void rain(LedStructure *leds, CommandParams cmd);
  void render(LedStructure *leds, CommandParams cmd);
  void ping_pong(LedStructure *leds, CommandParams cmd);
  void kaleidoscope(LedStructure *leds, CommandParams cmd);
  void mapped_slope(LedStructure *leds, CommandParams cmd);
  void mapped_shape(LedStructure *leds, CommandParams cmd);
  void mapped_triangle(LedStructure *leds, CommandParams cmd);
  void mapped_particles(LedStructure *leds, CommandParams cmd);
  void mapped_ping_pong(LedStructure *leds, CommandParams cmd);
  void gravity(LedStructure *leds, CommandParams cmd);
  void twang(LedStructure *leds);
  void fairy_light(LedStructure *leds, CommandParams cmd);
  void pixel_func(LedStructure *leds, CommandParams cmd);
  void debug(LedStructure *leds, CommandParams cmd);

  // Events
  void gravity_event(LedStructure *leds, uint8_t *data);

  // float easing functions
  static float ease_in_quad(float t);
  static float ease_out_quad(float t);
  static float ease_in_out_quad(float t);
  static float ease_in_cubic(float t);
  static float ease_out_cubic(float t);
  static float ease_in_out_cubic(float t);
  static uint8_t random_or_value(uint8_t value, uint8_t min, uint8_t max);

  // Render
  static void render(LedStructure *leds, Shape1D shape, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool bounce, bool wrap);
  static void render_ball(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool wrap);
  static void render_comet(LedStructure *leds, uint8_t strip_index, float position, float width, CRGB color, float brightness, bool direction, bool nyan, bool bounce, bool wrap);
  static void render_fill(LedStructure *leds, uint8_t strip_index, float position, CRGB color, float brightness, bool direction);

  // Mapped
  static void mapped_render_ball(LedStructure *leds, float x, float y, float size, CRGB color, float brightness, float fade_distance);
  static void render_mappled_slope(LedStructure *leds, float x1, float y1, float x2, float y2, SlopeType slope_type, float fade_distance, CRGB color, float brightness);

private:
  void render_commands();
};

#endif // __COMMANDS_H_INCLUDED__