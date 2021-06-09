#include <Tr33.h>

CRGB Tr33::trunk_leds[HW_TRUNK_STRIP_COUNT][HW_TRUNK_PIXEL_COUNT];
CRGB Tr33::branch_leds[BRANCH_STRIP_COUNT][BRANCH_PIXEL_COUNT];

Tr33::Tr33(void)
{
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_1>(trunk_leds[0], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_2>(trunk_leds[1], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_3>(trunk_leds[2], HW_TRUNK_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, TRUNK_PIN_4>(trunk_leds[3], HW_TRUNK_PIXEL_COUNT);

  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_1>(branch_leds[0], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_2>(branch_leds[1], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_3>(branch_leds[2], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_4>(branch_leds[3], BRANCH_PIXEL_COUNT);
  // not connected
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_5>(branch_leds[4], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_6>(branch_leds[5], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_7>(branch_leds[6], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_8>(branch_leds[7], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_9>(branch_leds[8], BRANCH_PIXEL_COUNT);
  // not connected
  // FastLED.addLeds<NEOPIXEL, BRANCH_PIN_10>(branch_leds[9], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_11>(branch_leds[10], BRANCH_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, BRANCH_PIN_12>(branch_leds[11], BRANCH_PIXEL_COUNT);
}

//
// -- Set leds ----------------------------------------
//

// strip index:
// 0 = all
// 1 = all_trunks
// 2 = all_branches
// 3-x = trunks
// x-y = branches

int trunk_index(int strip_index)
{
  return strip_index - STRIP_INDEX_ALL_BRANCHES;
}

bool is_trunk(int strip_index)
{
  return trunk_index(strip_index) > 0 && trunk_index(strip_index) < TRUNK_STRIP_COUNT;
}

int branch_index(int strip_index)
{
  return strip_index - STRIP_INDEX_ALL_BRANCHES - TRUNK_STRIP_COUNT;
}

bool is_branch(int strip_index)
{
  return branch_index(strip_index) > 0 && branch_index(strip_index) < BRANCH_STRIP_COUNT;
}

void set_trunk_led(int trunk, int led, CRGB color)
{
  if (trunk < HW_TRUNK_STRIP_COUNT)
  {
    Tr33::trunk_leds[trunk][TRUNK_PIXEL_COUNT - 1 - led] = color;
  }
  else
  {
    Tr33::trunk_leds[trunk - HW_TRUNK_STRIP_COUNT][led + TRUNK_PIXEL_COUNT] = color;
  }
}

CRGB get_trunk_led(int trunk, int led)
{
  if (trunk < HW_TRUNK_STRIP_COUNT)
  {
    return Tr33::trunk_leds[trunk][TRUNK_PIXEL_COUNT - 1 - led];
  }
  else
  {
    return Tr33::trunk_leds[trunk - HW_TRUNK_STRIP_COUNT][led + TRUNK_PIXEL_COUNT];
  }
}

uint16_t Tr33::strip_length(uint8_t strip_index)
{
  if (strip_index == STRIP_INDEX_ALL_TRUNKS || is_trunk(strip_index))
  {
    return TRUNK_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES || is_branch(strip_index))
  {
    return BRANCH_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return TRUNK_PIXEL_COUNT + BRANCH_PIXEL_COUNT;
  }
}

uint16_t Tr33::pixel_count(uint8_t strip_index)
{
  if (strip_index == STRIP_INDEX_ALL_TRUNKS || is_trunk(strip_index))
  {
    return TRUNK_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES || is_branch(strip_index))
  {
    return BRANCH_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return TRUNK_PIXEL_COUNT * TRUNK_STRIP_COUNT + BRANCH_PIXEL_COUNT * BRANCH_STRIP_COUNT;
  };
}

void Tr33::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    // single trunk
    if (is_trunk(strip_index))
    {
      set_trunk_led(trunk_index(strip_index), led, color);
    }
    // single branch
    else if (is_branch(strip_index))
    {
      branch_leds[branch_index(strip_index)][led] = color;
    }
  }
}

CRGB Tr33::get_led(uint8_t strip_index, int led)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    // single trunk
    if (is_trunk(strip_index))
    {
      return get_trunk_led(trunk_index(strip_index), led);
    }
    // single branch
    else if (is_branch(strip_index))
    {
      return branch_leds[branch_index(strip_index)][led];
    }
  }
}

void Tr33::fade_single_led(uint8_t strip_index, int led, CRGB target, fract8 amount)
{
  CRGB current = Tr33::get_led(strip_index, led);
  CRGB faded = blend(current, target, amount);
  Tr33::set_led(strip_index, led, faded);
}

void Tr33::fade_all_trunks(int led, CRGB target, fract8 amount)
{
  for (int i = 0; i < TRUNK_STRIP_COUNT; i++)
  {
    fade_single_led(i + STRIP_INDEX_ALL_BRANCHES, led, target, amount);
  }
}

void Tr33::fade_all_branches(int led, CRGB target, fract8 amount)
{
  for (int i = 0; i < BRANCH_STRIP_COUNT; i++)
  {
    fade_single_led(i + STRIP_INDEX_ALL_BRANCHES + TRUNK_STRIP_COUNT, led, target, amount);
  }
}

void Tr33::fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount)
{
  if (strip_index == STRIP_INDEX_ALL)
  {
    if (led < TRUNK_PIXEL_COUNT)
    {
      fade_all_trunks(led, target, amount);
    }
    else
    {
      fade_all_branches(led - TRUNK_PIXEL_COUNT, target, amount);
    }
  }
  else if (strip_index == STRIP_INDEX_ALL_TRUNKS)
  {
    fade_all_trunks(led, target, amount);
  }
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES)
  {
    fade_all_branches(led, target, amount);
  }
  else
  {
    fade_single_led(strip_index, led, target, amount);
  }
}

//
// -- Helper ------------------------------------------------------
//

uint8_t Tr33::random_strip(uint8_t strip_index)
{
  if (is_trunk(strip_index) || is_branch(strip_index))
  {
    return strip_index;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return random8(0, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT) + STRIP_INDEX_ALL_BRANCHES + 1;
  }
  else if (strip_index == STRIP_INDEX_ALL_TRUNKS)
  {
    return random8(0, TRUNK_STRIP_COUNT) + STRIP_INDEX_ALL_BRANCHES + 1;
  }
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES)
  {
    return random8(0, BRANCH_STRIP_COUNT) + TRUNK_STRIP_COUNT + STRIP_INDEX_ALL_BRANCHES + 1;
  }
}

void show_pin_numbers()
{
  // Serial.println("Showing pin Numbers");

  // command_buffer[0].type = SHOW_NUMBER;
  // command_buffer[0].data[0] = 0;
  // command_buffer[0].data[1] = TRUNK_PIN_1;

  // command_buffer[1].type = SHOW_NUMBER;
  // command_buffer[1].data[0] = 1;
  // command_buffer[1].data[1] = TRUNK_PIN_2;

  // command_buffer[2].type = SHOW_NUMBER;
  // command_buffer[2].data[0] = 2;
  // command_buffer[2].data[1] = TRUNK_PIN_3;

  // command_buffer[3].type = SHOW_NUMBER;
  // command_buffer[3].data[0] = 3;
  // command_buffer[3].data[1] = TRUNK_PIN_4;

  // command_buffer[4].type = SHOW_NUMBER;
  // command_buffer[4].data[0] = TRUNK_STRIP_COUNT;
  // command_buffer[4].data[1] = BRANCH_PIN_1;

  // command_buffer[5].type = SHOW_NUMBER;
  // command_buffer[5].data[0] = TRUNK_STRIP_COUNT + 1;
  // command_buffer[5].data[1] = BRANCH_PIN_2;

  // command_buffer[6].type = SHOW_NUMBER;
  // command_buffer[6].data[0] = TRUNK_STRIP_COUNT + 2;
  // command_buffer[6].data[1] = BRANCH_PIN_3;

  // command_buffer[7].type = SHOW_NUMBER;
  // command_buffer[7].data[0] = TRUNK_STRIP_COUNT + 3;
  // command_buffer[7].data[1] = BRANCH_PIN_4;

  // command_buffer[8].type = SHOW_NUMBER;
  // command_buffer[8].data[0] = TRUNK_STRIP_COUNT + 4;
  // command_buffer[8].data[1] = BRANCH_PIN_5;

  // command_buffer[9].type = SHOW_NUMBER;
  // command_buffer[9].data[0] = TRUNK_STRIP_COUNT + 5;
  // command_buffer[9].data[1] = BRANCH_PIN_6;

  // command_buffer[10].type = SHOW_NUMBER;
  // command_buffer[10].data[0] = TRUNK_STRIP_COUNT + 6;
  // command_buffer[10].data[1] = BRANCH_PIN_7;

  // command_buffer[11].type = SHOW_NUMBER;
  // command_buffer[11].data[0] = TRUNK_STRIP_COUNT + 7;
  // command_buffer[11].data[1] = BRANCH_PIN_8;

  // command_buffer[12].type = SHOW_NUMBER;
  // command_buffer[12].data[0] = TRUNK_STRIP_COUNT + 8;
  // command_buffer[12].data[1] = BRANCH_PIN_9;

  // command_buffer[13].type = SHOW_NUMBER;
  // command_buffer[13].data[0] = TRUNK_STRIP_COUNT + 9;
  // command_buffer[13].data[1] = BRANCH_PIN_10;

  // command_buffer[14].type = SHOW_NUMBER;
  // command_buffer[14].data[0] = TRUNK_STRIP_COUNT + 10;
  // command_buffer[14].data[1] = BRANCH_PIN_11;

  // command_buffer[15].type = SHOW_NUMBER;
  // command_buffer[15].data[0] = TRUNK_STRIP_COUNT + 11;
  // command_buffer[15].data[1] = BRANCH_PIN_12;
}