#ifndef __LEDSTRUCTURE_H_INCLUDED__
#define __LEDSTRUCTURE_H_INCLUDED__

#include <array>
#include <Arduino.h>
#include <FastLED.h>
#include <command_schemas.pb.h>
#include <FairyLightsControl.h>

#ifdef LED_STRUCTURE_WAND
#include <Wand_config.h>
#endif
#ifdef LED_STRUCTURE_NEON
#include <Neon_config.h>
#endif
#ifdef LED_STRUCTURE_TR33
#include <Tr33_config.h>
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
#ifdef LED_STRUCTURE_SCUBAR
#include <Scubar_config.h>
#endif
#ifdef LED_STRUCTURE_TETRAEDER
#include <Tetraeder_config.h>
#endif
#ifdef LED_STRUCTURE_ARROW
#include <Arrow_config.h>
#endif
#ifdef LED_STRUCTURE_MINICHROME
#include <Minichrome_config.h>
#endif

class LedStructure
{
private:
  static const float mapping[][4];

public:
  static CRGB leds[STRIP_COUNT][STRIP_PIXEL_COUNT];
  virtual void init();

  // mapping
  uint8_t mapping_sprip_index(uint16_t index);
  int mapping_led(uint16_t index);
  float mapping_x(uint16_t index);
  float mapping_y(uint16_t index);
  uint16_t mapping_size();

  // set leds
  virtual void set_led(uint8_t strip_index, int led, CRGB color);
  virtual CRGB get_led(uint8_t strip_index, int led);
  virtual void fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount);
  virtual void fade_led(CommandParams cmd, int led, CRGB target);
  virtual void shift_led(uint8_t strip_index, int led, uint8_t amount);
  virtual void invert_led(uint8_t strip_index, int led, uint8_t amount);
  virtual uint8_t random_strip(uint8_t strip_index);
  virtual uint16_t strip_length(uint8_t strip_index);
  virtual uint16_t pixel_count(uint8_t strip_index);

  // fairy lights
  virtual FairyLightsControl *get_fairy_light(uint8_t index);

  // events - rendered once
  void joystick(uint8_t *data);

  // misc
  virtual String get_name();
  virtual uint8_t get_max_strip_index();
};

#endif // __LEDSTRUCTURE_H_INCLUDED__