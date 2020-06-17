#ifndef __LEDSTRUCTURE_H_INCLUDED__
#define __LEDSTRUCTURE_H_INCLUDED__

#include <Arduino.h>
#include <FastLED.h>

#ifdef LED_STRUCTURE_WAND
#include <Wand_config.h>
#endif
#ifdef LED_STRUCTURE_TR33
#include <Tr33_config.h>
#endif
#ifdef LED_STRUCTURE_KELLER
#include <Keller_config.h>
#endif

class LedStructure
{
public:
  static CRGB leds[STRIP_COUNT][STRIP_PIXEL_COUNT];
  static float mapping[][4];
  virtual void init();

  // set leds
  virtual void set_led(uint8_t strip_index, int led, CRGB color);
  virtual CRGB get_led(uint8_t strip_index, int led);
  virtual void fade_led(uint8_t strip_index, int led, CRGB target, float amount);
  virtual uint8_t random_strip(uint8_t strip_index);
  virtual uint16_t strip_length(uint8_t strip_index);
  virtual uint8_t strip_index_all();

  // commands
  void gravity(char *data);
  void beats(char *data);
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

#endif // __LEDSTRUCTURE_H_INCLUDED__