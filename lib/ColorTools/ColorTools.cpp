#include <ColorTools.h>
#include <cmath>
#include <Commands.h>
#include <Network.h>
#include <ESPxRGB.h>

const uint8_t gamma8[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

// generate a color that has a fixed distance in CIE94 deltaE. This represents the human perseption of difference in color.
// ranges for deltaE.
// Range: 5 to 85
std::array<CRGB, 2> ColorTools::random_color_pair(float target_distance)
{
  float tolerance = 0.05f;

  float distance;
  std::array<CRGB, 2> colors;

  target_distance = fmaxf(5.0f, fminf(target_distance, 85.0f));
  colors[0] = random_color();

  do
  {
    colors[1] = random_color();
    distance = cie94_distance(colors[0], colors[1]);

  } while (fabs(distance - target_distance) > tolerance * target_distance);

  // Network::remote_log("Distance: " + String(distance) + " Target: " + String(target_distance) + " Tolerance: " + String(tolerance * target_distance));
  // Network::remote_log("Colors: " + String(colors[0].r) + " " + String(colors[0].g) + " " + String(colors[0].b) + " and " + String(colors[1].r) + " " + String(colors[1].g) + " " + String(colors[1].b));

  return colors;
}

// Get random color within a sensible saturation range and max value/brightness
CRGB ColorTools::random_color()
{
  uint8_t min_saturation = 150;
  uint8_t min_value = 255;

  uint16_t hue = random(0, HSV_HUE_MAX);
  uint8_t saturation = random(min_saturation, HSV_SAT_MAX);
  uint8_t value = random(min_value, HSV_VAL_MAX);

  CRGB result;
  xhsv2rgb8(hue, saturation, value, &result.r, &result.g, &result.b);

  return result;
}

float ColorTools::cie94_distance(CRGB &color1, CRGB &color2)
{
  CLAB lab1 = rgb_to_lab(color1);
  CLAB lab2 = rgb_to_lab(color2);
  return cie94_distance(lab1, lab2);
}

// CIE94 calculation
float ColorTools::cie94_distance(CLAB &color1, CLAB &color2)
{
  const float kL = 1.0f, kC = 1.0f, kH = 1.0f;
  const float C1 = std::sqrt(color1.a * color1.a + color1.b * color1.b);
  const float C2 = std::sqrt(color2.a * color2.a + color2.b * color2.b);
  const float deltaL = color1.L - color2.L;
  const float deltaC = C1 - C2;
  const float deltaH = std::sqrt((color1.a - color2.a) * (color1.a - color2.a) +
                                 (color1.b - color2.b) * (color1.b - color2.b) -
                                 deltaC * deltaC);

  const float SL = 1.0f;
  const float SC = 1.0f + 0.045f * C1;
  const float SH = 1.0f + 0.015f * C1;

  float deltaE = std::sqrt(
      std::pow(deltaL / (kL * SL), 2) +
      std::pow(deltaC / (kC * SC), 2) +
      std::pow(deltaH / (kH * SH), 2));

  return deltaE;
}

void ColorTools::gamma_correction(CRGB &color)
{
  if (Commands::firmware_config.gamma_correction)
  {
    color.r = pgm_read_byte(&gamma8[color.r]);
    color.g = pgm_read_byte(&gamma8[color.g]);
    color.b = pgm_read_byte(&gamma8[color.b]);
  }
}

ColorTools::CLAB ColorTools::rgb_to_lab(CRGB color)
{
  // Convert RGB to linear RGB
  float R = rgb_to_linear(float(color.r) / 255.0f);
  float G = rgb_to_linear(float(color.g) / 255.0f);
  float B = rgb_to_linear(float(color.b) / 255.0f);

  // Convert linear RGB to XYZ
  float X, Y, Z;
  linear_to_xyz(R, G, B, X, Y, Z);

  // Normalize XYZ values with respect to D65 reference white
  X /= 0.95047f;
  Y /= 1.00000f; // D65 illuminant
  Z /= 1.08883f;

  // Convert XYZ to CIELAB
  CLAB result;
  result.L = 116.0f * cielab_f(Y) - 16.0f;
  result.a = 500.0f * (cielab_f(X) - cielab_f(Y));
  result.b = 200.0f * (cielab_f(Y) - cielab_f(Z));

  return result;
}

// Blends colors through the RGB space. This has much more visible color changes than RGB blending.
CHSV ColorTools::blend_hsv(CHSV a, CHSV b, float amount)
{
  CHSV result;
  result.h = blend8(a.h, b.h, amount);
  result.s = blend8(a.s, b.s, amount);
  result.v = blend8(a.v, b.v, amount);
  return result;
}

// private functions

// Convert a single RGB channel from sRGB to linear RGB
float ColorTools::rgb_to_linear(float channel)
{
  return (channel <= 0.04045f) ? channel / 12.92f : std::pow((channel + 0.055f) / 1.055f, 2.4f);
}

// Convert linear RGB to XYZ
void ColorTools::linear_to_xyz(float R, float G, float B, float &X, float &Y, float &Z)
{
  // Assuming sRGB color space and D65 illuminant
  X = R * 0.4124564f + G * 0.3575761f + B * 0.1804375f;
  Y = R * 0.2126729f + G * 0.7151522f + B * 0.0721750f;
  Z = R * 0.0193339f + G * 0.1191920f + B * 0.9503041f;
}

// Function to apply the CIELAB conversion formula
float ColorTools::cielab_f(float t)
{
  return (t > std::pow(6.0f / 29.0f, 3)) ? std::pow(t, 1.0f / 3.0f) : 7.787f * t + 16.0f / 116.0f;
}

uint8_t ColorTools::blend8(uint8_t a, uint8_t b, float t)
{
  if (t < 0.0f)
    t = 0.0f;
  if (t > 1.0f)
    t = 1.0f;

  const float result = a + t * (b - a);

  return static_cast<uint8_t>(std::floor(result + 0.5f));
}
