#include <Commands.h>

#ifndef LEDS_H
#define LEDS_H
#include <LedStructure.h>
#endif

LedStructure::LedStructure()
{
  // todo: find a better way to do this
  // #ifdef LED_STRUCTURE_BASE_HOME
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(strip_leds[0], STRIP_PIXEL_COUNT);
  // #endif

#ifdef LED_STRUCTURE_BASE_KELLER
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(strip_leds[0], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(strip_leds[1], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(strip_leds[2], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_4>(strip_leds[3], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_5>(strip_leds[4], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_6>(strip_leds[5], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_7>(strip_leds[6], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_8>(strip_leds[7], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_9>(strip_leds[8], STRIP_PIXEL_COUNT);
  // FastLED.addLeds<NEOPIXEL, STRIP_PIN_10>(strip_leds[9], STRIP_PIXEL_COUNT);
#endif
}

void LedStructure::init()
{
  command_buffer[0].type = COMMAND_RAINBOW_SINE;
  command_buffer[0].data[0] = STRIP_INDEX_ALL;
  command_buffer[0].data[1] = 10;
  command_buffer[0].data[2] = 100;
  command_buffer[0].data[3] = 100;
  command_buffer[0].data[4] = 255;
}

//
// -- Set leds ----------------------------------------
//

void LedStructure::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index < STRIP_COUNT)
  {
    strip_leds[strip_index][led] = color;
  }
  else
  {
    for (int i = 0; i < STRIP_COUNT; i++)
    {
      strip_leds[i][led] = color;
    }
  }
}

CRGB LedStructure::get_led(uint8_t strip_index, int led)
{

  if (strip_index < STRIP_COUNT)
  {
    return strip_leds[strip_index][led];
  }
  else
  {
    return strip_leds[0][led];
  }
}

void LedStructure::fade_led(uint8_t strip_index, int led, CRGB target, float amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(strip_index, led, faded);
  }
}

uint16_t LedStructure::strip_length(uint8_t strip_index)
{
  return STRIP_PIXEL_COUNT;
}

uint8_t LedStructure::random_strip(uint8_t strip_index)
{
  if (strip_index < STRIP_COUNT)
  {
    return strip_index;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return random8(0, STRIP_COUNT);
  }
}

uint8_t LedStructure::strip_index_all()
{
  return STRIP_INDEX_ALL;
}

// void Tr33::show_pin_numbers()
// {
//   // Serial.println("Showing pin Numbers");

//   // command_buffer[0].type = SHOW_NUMBER;
//   // command_buffer[0].data[0] = 0;
//   // command_buffer[0].data[1] = TRUNK_PIN_1;

//   // command_buffer[1].type = SHOW_NUMBER;
//   // command_buffer[1].data[0] = 1;
//   // command_buffer[1].data[1] = TRUNK_PIN_2;

//   // command_buffer[2].type = SHOW_NUMBER;
//   // command_buffer[2].data[0] = 2;
//   // command_buffer[2].data[1] = TRUNK_PIN_3;

//   // command_buffer[3].type = SHOW_NUMBER;
//   // command_buffer[3].data[0] = 3;
//   // command_buffer[3].data[1] = TRUNK_PIN_4;

//   // command_buffer[4].type = SHOW_NUMBER;
//   // command_buffer[4].data[0] = TRUNK_STRIP_COUNT;
//   // command_buffer[4].data[1] = STRIP_PIN_1;

//   // command_buffer[5].type = SHOW_NUMBER;
//   // command_buffer[5].data[0] = TRUNK_STRIP_COUNT + 1;
//   // command_buffer[5].data[1] = STRIP_PIN_2;

//   // command_buffer[6].type = SHOW_NUMBER;
//   // command_buffer[6].data[0] = TRUNK_STRIP_COUNT + 2;
//   // command_buffer[6].data[1] = STRIP_PIN_3;

//   // command_buffer[7].type = SHOW_NUMBER;
//   // command_buffer[7].data[0] = TRUNK_STRIP_COUNT + 3;
//   // command_buffer[7].data[1] = STRIP_PIN_4;

//   // command_buffer[8].type = SHOW_NUMBER;
//   // command_buffer[8].data[0] = TRUNK_STRIP_COUNT + 4;
//   // command_buffer[8].data[1] = STRIP_PIN_5;

//   // command_buffer[9].type = SHOW_NUMBER;
//   // command_buffer[9].data[0] = TRUNK_STRIP_COUNT + 5;
//   // command_buffer[9].data[1] = STRIP_PIN_6;

//   // command_buffer[10].type = SHOW_NUMBER;
//   // command_buffer[10].data[0] = TRUNK_STRIP_COUNT + 6;
//   // command_buffer[10].data[1] = STRIP_PIN_7;

//   // command_buffer[11].type = SHOW_NUMBER;
//   // command_buffer[11].data[0] = TRUNK_STRIP_COUNT + 7;
//   // command_buffer[11].data[1] = STRIP_PIN_8;

//   // command_buffer[12].type = SHOW_NUMBER;
//   // command_buffer[12].data[0] = TRUNK_STRIP_COUNT + 8;
//   // command_buffer[12].data[1] = STRIP_PIN_9;

//   // command_buffer[13].type = SHOW_NUMBER;
//   // command_buffer[13].data[0] = TRUNK_STRIP_COUNT + 9;
//   // command_buffer[13].data[1] = STRIP_PIN_10;

//   // command_buffer[14].type = SHOW_NUMBER;
//   // command_buffer[14].data[0] = TRUNK_STRIP_COUNT + 10;
//   // command_buffer[14].data[1] = STRIP_PIN_11;

//   // command_buffer[15].type = SHOW_NUMBER;
//   // command_buffer[15].data[0] = TRUNK_STRIP_COUNT + 11;
//   // command_buffer[15].data[1] = STRIP_PIN_12;
// }

void LedStructure::gravity(char *data)
{
  return;
}

void LedStructure::beats(char *data)
{
  return;
}

void LedStructure::mapped_swipe(char *data)
{
  return;
}

void LedStructure::mapped_shape(char *data)
{
  return;
}

void LedStructure::random_walk(char *data)
{
  return;
}

void LedStructure::debug(char *data)
{
  return;
}

void LedStructure::fireworks(char *data)
{
  return;
}

void LedStructure::rotating_sectors(char *data)
{
  return;
}

void LedStructure::rotating_plane(char *data)
{
  return;
}

void LedStructure::twang(char *data)
{
  return;
}

void LedStructure::gravity_event()
{
  return;
}

void LedStructure::joystick(char *data)
{
  return;
}

void LedStructure::write_info()
{
  Serial.println("Generic Led Structure");
  return;
}
