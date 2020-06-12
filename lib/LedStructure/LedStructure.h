#include <Arduino.h>
#include <FastLED.h>

#ifdef LED_STRUCTURE_BASE_HOME
#include <Structures/Home.h>
#endif

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70
#define MAX_RAIN_DROPS 500

#define SWIPE_TOP_BOTTOM 0
#define SWIPE_BOTTOM_TOP 1
#define SWIPE_LEFT_RIGHT 2
#define SWIPE_RIGHT_LEFT 3

#define SHAPE_SQUARE 0
#define SHAPE_HOLLOW_SQUARE 1
#define SHAPE_CIRCLE 2

struct Command;

class LedStructure
{
private:
  CRGB strip_leds[STRIP_COUNT][STRIP_PIXEL_COUNT];

public:
  LedStructure();
  void init();

  // set leds
  void set_led(uint8_t strip_index, int led, CRGB color);
  CRGB get_led(uint8_t strip_index, int led);
  void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  uint8_t random_strip(uint8_t strip_index);
  uint16_t strip_length(uint8_t strip_index);
  uint8_t strip_index_all();

  // commands
  void gravity(char *data);
  void beats(char *data);
  void mapped_swipe(char *data);
  void mapped_shape(char *data);
  void random_walk(char *data);
  void debug(char *data);
  void fireworks(char *data);
  void rotating_sectors(char *data);
  void rotating_plane(char *data);
  void twang(char *data);

  // events - rendered once
  void gravity_event();
  void joystick(char *data);

  // artnet
  static void artnet_packet_callback(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);

  // misc
  virtual void write_info();
};