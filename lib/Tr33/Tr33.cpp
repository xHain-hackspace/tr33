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
  if (strip_index < TRUNK_STRIP_COUNT || strip_index == STRIP_INDEX_ALL_TRUNKS)
  {
    return TRUNK_PIXEL_COUNT;
  }
  else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT || strip_index == STRIP_INDEX_ALL_BRANCHES)
  {
    return BRANCH_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return TRUNK_PIXEL_COUNT + BRANCH_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_SPIRAL)
  {
    return 100;
  }
}

int spiral_strip(uint8_t index)
{
  return index % 4;
}
int spiral_led_index(uint8_t index)
{
  return index;
}

void Tr33::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    // single trunk
    if (strip_index < TRUNK_STRIP_COUNT)
    {
      set_trunk_led(strip_index, led, color);
      // single branch
    }
    else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT)
    {
      branch_leds[strip_index - TRUNK_STRIP_COUNT][led] = color;
      // all trunks
    }
    else if (strip_index == STRIP_INDEX_ALL_TRUNKS)
    {
      for (int i = 0; i < TRUNK_STRIP_COUNT; i++)
      {
        set_trunk_led(i, led, color);
      }
      // all branches
    }
    else if (strip_index == STRIP_INDEX_ALL_BRANCHES)
    {
      for (int i = 0; i < BRANCH_STRIP_COUNT; i++)
      {
        branch_leds[i][led] = color;
      }
      // all trunks and branches
    }
    else if (strip_index == STRIP_INDEX_ALL)
    {
      if (led < TRUNK_PIXEL_COUNT)
      {
        for (int i = 0; i < TRUNK_STRIP_COUNT; i++)
        {
          set_trunk_led(i, led, color);
        }
      }
      else
      {
        for (int i = 0; i < BRANCH_STRIP_COUNT; i++)
        {
          branch_leds[i][led - TRUNK_PIXEL_COUNT] = color;
        }
      }
      // spiral
    }
    else if (strip_index == STRIP_INDEX_SPIRAL)
    {
      set_trunk_led(spiral_strip(led), spiral_led_index(led), color);
    }
  }
}

CRGB Tr33::get_led(uint8_t strip_index, int led)
{
  // single trunk
  if (strip_index < TRUNK_STRIP_COUNT)
  {
    return get_trunk_led(strip_index, led);
  }
  // single branch
  else if (strip_index < TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT)
  {
    return branch_leds[strip_index - TRUNK_STRIP_COUNT][led];
  }
  // all trunks
  else if (strip_index == STRIP_INDEX_ALL_TRUNKS)
  {
    return get_trunk_led(0, led);
  }
  // all branches
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES)
  {
    return branch_leds[0][led];
  }
  // all trunks and branches
  else if (strip_index == STRIP_INDEX_ALL)
  {
    if (led < TRUNK_PIXEL_COUNT)
    {
      return get_trunk_led(0, led);
    }
    else
    {
      return branch_leds[0][led - TRUNK_PIXEL_COUNT];
    }
    // spiral
  }
  else if (strip_index == STRIP_INDEX_SPIRAL)
  {
    return get_trunk_led(spiral_strip(led), spiral_led_index(led));
  }
}

//
// -- Helper ------------------------------------------------------
//

uint8_t Tr33::random_strip(uint8_t strip_index)
{
  if (strip_index < BRANCH_STRIP_COUNT + TRUNK_STRIP_COUNT)
  {
    return strip_index;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return random8(0, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT);
  }
  else if (strip_index == STRIP_INDEX_ALL_TRUNKS)
  {
    return random8(0, TRUNK_STRIP_COUNT);
  }
  else if (strip_index == STRIP_INDEX_ALL_BRANCHES)
  {
    return random8(TRUNK_STRIP_COUNT, TRUNK_STRIP_COUNT + BRANCH_STRIP_COUNT);
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