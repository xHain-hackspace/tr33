#include <Keller.h>
#include <Commands.h>

CRGB strip_leds[STRIP_COUNT][STRIP_PIXEL_COUNT];

Keller::Keller(void)
{
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_1>(strip_leds[0], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_2>(strip_leds[1], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_3>(strip_leds[2], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_4>(strip_leds[3], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_5>(strip_leds[4], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_6>(strip_leds[5], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_7>(strip_leds[6], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_8>(strip_leds[7], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_9>(strip_leds[8], STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, STRIP_PIN_10>(strip_leds[9], STRIP_PIXEL_COUNT);
}

void Keller::init()
{

  // uncomment this to show the pin number on each strip
  // show_pin_numbers();

  command_buffer[0].type = COMMAND_SINGLE_COLOR;
  command_buffer[0].data[0] = STRIP_INDEX_ALL;
  command_buffer[0].data[1] = HUE_BLUE;
  command_buffer[0].data[2] = 120;

  // command_buffer[1].type = COMMAND_FILL;
  // command_buffer[1].data[0] = STRIP_INDEX_ALL;
  // command_buffer[1].data[1] = FILL_BALL;
  // command_buffer[1].data[2] = HUE_ORANGE;
  // command_buffer[1].data[3] = 255;
  // command_buffer[1].data[4] = 120;
  // command_buffer[1].data[5] = 50;

  command_buffer[2].type = COMMAND_PING_PONG;
  command_buffer[2].data[0] = RENDER_NYAN_BOUNCE;
  // command_buffer[2].data[0] = RENDER_COMET_BOUNCE;
  command_buffer[2].data[1] = STRIP_INDEX_ALL;
  command_buffer[2].data[2] = HUE_RED;
  command_buffer[2].data[3] = 255;
  command_buffer[2].data[4] = 100;
  command_buffer[2].data[5] = PING_PONG_LINEAR;
  command_buffer[2].data[6] = 70;
  command_buffer[2].data[7] = 0;
  command_buffer[2].data[8] = 255;

  // command_buffer[1].type = MAPPED_SWIPE;
  // command_buffer[1].data[0] = SWIPE_BOTTOM_TOP;
  // command_buffer[1].data[1] = 200;
  // command_buffer[1].data[2] = 100;

  // command_buffer[1].type = MAPPED_SHAPE;
  // command_buffer[1].data[0] = SHAPE_HOLLOW_SQUARE;
  // command_buffer[1].data[1] = 50;
  // command_buffer[1].data[2] = 128;
  // command_buffer[1].data[3] = 128;
  // command_buffer[1].data[4] = 50;

  // command_buffer[0].type = MAPPED_SWIPE;
  // command_buffer[0].data[0] = 200;
  // command_buffer[0].data[1] = 100;
  // command_buffer[0].data[2] = SWIPE_BOTTOM_TOP;

  // command_buffer[1].type = COMMAND_PING_PONG;
  // command_buffer[1].data[0] = STRIP_INDEX_ALL;
  // command_buffer[1].data[1] = 1;
  // command_buffer[1].data[2] = 1;
  // command_buffer[1].data[3] = 255;
  // command_buffer[1].data[4] = 190;
  // command_buffer[1].data[5] = 20;
  // command_buffer[1].data[6] = 0;

  // command_buffer[0].type = BEATS;
  // command_buffer[0].data[0] = STRIP_INDEX_ALL_STRIPES;
  // command_buffer[0].data[1] = 0;
  // command_buffer[0].data[2] = 20;

  // command_buffer[0].type = COMMAND_RAINBOW_SINE;
  // command_buffer[0].data[0] = STRIP_INDEX_ALL;
  // command_buffer[0].data[1] = 60;
  // command_buffer[0].data[2] = 50;
  // command_buffer[0].data[3] = 50;
  // command_buffer[0].data[4] = 255;

  // command_buffer[0].type = COMMAND_TWANG;
  // command_buffer[0].data[0] = 0;
}

//
// -- Set leds ----------------------------------------
//

uint16_t Keller::strip_length(uint8_t strip_index)
{
  return STRIP_PIXEL_COUNT;
}

void Keller::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index < STRIP_COUNT)
  {
    strip_leds[strip_index][led] = color;
  }
  if (strip_index == STRIP_COUNT)
  {
    for (int i = 0; i < STRIP_COUNT; i++)
    {
      strip_leds[i][led] = color;
    }
  }
}

CRGB get_led(uint8_t strip_index, int led)
{

  if (strip_index < STRIP_COUNT)
  {
    return strip_leds[strip_index][led];
  }
  if (strip_index == STRIP_COUNT)
  {
    return strip_leds[0][led];
  }
}

void Keller::fade_led(uint8_t strip_index, int led, CRGB target, float amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(strip_index, led, faded);
  }
}

void Keller::all_white()
{
  for (int i = 0; i < strip_length(STRIP_INDEX_ALL); i++)
  {
    set_led(STRIP_INDEX_ALL, i, CRGB(255, 255, 255));
  }
}

//
// -- Helper ------------------------------------------------------
//

uint8_t Keller::random_strip(uint8_t strip_index)
{
  return random8(0, STRIP_COUNT);
}

uint8_t Keller::strip_count()
{
  return STRIP_COUNT;
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