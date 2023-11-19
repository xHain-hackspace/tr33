#ifndef __COLORTOOLS_H__INCLUDED__
#define __COLORTOOLS_H__INCLUDED__

#include <FastLED.h>

class ColorTools
{
public:
  struct CLAB
  {
    float L, a, b;
  };

  static void gamma_correction(CRGB &color);
  static std::array<CRGB, 2> random_color_pair(float distance);
  static CRGB random_color();

  static float cie94_distance(CRGB &color1, CRGB &color2);
  static float cie94_distance(CLAB &color1, CLAB &color2);
  static CLAB rgb_to_lab(CRGB color);

  CHSV static blend_hsv(CHSV a, CHSV b, float amount);

private:
  static float rgb_to_linear(float channel);
  static void linear_to_xyz(float R, float G, float B, float &X, float &Y, float &Z);
  static float cielab_f(float t);

  static uint8_t blend8(uint8_t a, uint8_t b, float t);
};

#endif // __COLORTOOLS_H__INCLUDED__
