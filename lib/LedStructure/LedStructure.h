#ifndef __LEDSTRUCTURE_H_INCLUDED__
#define __LEDSTRUCTURE_H_INCLUDED__

#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>

#ifdef LED_STRUCTURE_WAND
#include <Wand_config.h>
#endif
#ifdef LED_STRUCTURE_TR33
#include <Tr33_config.h>
#endif
#ifdef LED_STRUCTURE_KELLER
#include <Keller_config.h>
#endif
#ifdef LED_STRUCTURE_RANKEN
#include <Ranken_config.h>
#endif
#ifdef LED_STRUCTURE_TROMMEL
#include <Trommel_config.h>
#endif
#ifdef LED_STRUCTURE_WOLKE
#include <Wolke_config.h>
#endif
#ifdef LED_STRUCTURE_DISCOTESTRIG
#include <Discotestrig_config.h>
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
  virtual void fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount);
  virtual void fade_led(CommandParams cmd, int led, CRGB target);
  virtual uint8_t random_strip(uint8_t strip_index);
  virtual uint16_t strip_length(uint8_t strip_index);
  virtual uint16_t pixel_count(uint8_t strip_index);

  // commands
  void beats(uint8_t *data);
  void random_walk(uint8_t *data);
  void debug(uint8_t *data);
  void fireworks(uint8_t *data);
  void rotating_sectors(uint8_t *data);
  void rotating_plane(uint8_t *data);
  void twang(uint8_t *data);

  // events - rendered once
  void joystick(uint8_t *data);

  // artnet
  static void artnet_packet_callback(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data);

  // misc
  virtual void write_info();
};

#endif // __LEDSTRUCTURE_H_INCLUDED__