#include <Dode.h>
#include <Commands.h>

CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

Dode::Dode(void)
{
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_0>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_1>(edge_leds[1], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_2>(edge_leds[2], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_3>(edge_leds[3], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_4>(edge_leds[4], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_5>(edge_leds[5], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_6>(edge_leds[6], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_7>(edge_leds[7], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_8>(edge_leds[8], HW_STRIP_PIXEL_COUNT);
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_9>(edge_leds[9], HW_STRIP_PIXEL_COUNT);
}

void Dode::init()
{
  generate_mapping(); //generate cartesian and spherical coordinates of pixels

  //  command_buffer[12].type = COMMAND_WHITE;

  // command_buffer[0].type = COMMAND_SINGLE_COLOR;
  // command_buffer[0].data[0] = 31;
  // command_buffer[0].data[1] = HUE_PURPLE;
  // command_buffer[0].data[2] = 20;

  // command_buffer[5].type = COMMAND_ROTATING_SECTORS;
  // command_buffer[5].data[0] = HUE_ORANGE;
  // command_buffer[5].data[1] = 255;
  // command_buffer[5].data[2] = 255;//*100ms
  // command_buffer[5].data[3] = 0;//255 =360 degrees
  // command_buffer[5].data[4] = 1;
  // command_buffer[5].data[5] = 10;//255 =360 degrees

  // command_buffer[9].type = COMMAND_ROTATING_SECTORS;
  // command_buffer[9].data[0] = HUE_RED;
  // command_buffer[9].data[1] = 255;
  // command_buffer[9].data[2] = 255;//*100ms
  // command_buffer[9].data[3] = 128;//255 =360 degrees
  // command_buffer[9].data[4] = 4;
  // command_buffer[9].data[5] = 5;//255 =360 degrees

  // command_buffer[1].type = COMMAND_KALEIDOSCOPE;
  // command_buffer[1].data[0] = HUE_ORANGE;
  // command_buffer[1].data[1] = 255;
  // command_buffer[1].data[2] = 20;

  // command_buffer[8].type = COMMAND_RANDOM_WALK;
  // command_buffer[8].data[0] = HUE_RED;
  // command_buffer[8].data[1] = 255;
  // command_buffer[8].data[2] = 27;
  // command_buffer[8].data[3] = 100;
  // command_buffer[8].data[4] = 5;
  // command_buffer[8].data[5] = BALL_TYPE_NYAN;

  // command_buffer[0].type = COMMAND_SPARKLE;
  // command_buffer[0].data[0] = 31;
  // command_buffer[0].data[1] = 31;
  // command_buffer[0].data[2] = 30;
  // command_buffer[0].data[3] = 200;

  // command_buffer[2].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[2].data[0] = HUE_AQUA;
  // command_buffer[2].data[1] = 100;
  // command_buffer[2].data[2] = 50;
  // command_buffer[2].data[3] = 0;
  // command_buffer[2].data[4] = 5;
  // command_buffer[2].data[5] = SWIPE_X;

  // command_buffer[3].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[3].data[0] = HUE_ORANGE;
  // command_buffer[3].data[1] = 100;
  // command_buffer[3].data[2] = 50;
  // command_buffer[3].data[3] = 0;
  // command_buffer[3].data[4] = 5;
  // command_buffer[3].data[5] = SWIPE_nX;

  // command_buffer[4].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[4].data[0] = HUE_AQUA;
  // command_buffer[4].data[1] = 100;
  // command_buffer[4].data[2] = 50;
  // command_buffer[4].data[3] = 0;
  // command_buffer[4].data[4] = 5;
  // command_buffer[4].data[5] = SWIPE_Y;

  // command_buffer[5].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[5].data[0] = HUE_ORANGE;
  // command_buffer[5].data[1] = 100;
  // command_buffer[5].data[2] = 50;
  // command_buffer[5].data[3] = 0;
  // command_buffer[5].data[4] = 5;
  // command_buffer[5].data[5] = SWIPE_nY;

  // command_buffer[6].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[6].data[0] = HUE_AQUA;
  // command_buffer[6].data[1] = 100;
  // command_buffer[6].data[2] = 50;
  // command_buffer[6].data[3] = 0;
  // command_buffer[6].data[4] = 10;
  // command_buffer[6].data[5] = SWIPE_X;

  // command_buffer[7].type = COMMAND_MAPPED_SWIPE;
  // command_buffer[7].data[0] = HUE_ORANGE;
  // command_buffer[7].data[1] = 100;
  // command_buffer[7].data[2] = 50;
  // command_buffer[7].data[3] = 0;
  // command_buffer[7].data[4] = 10;
  // command_buffer[7].data[5] = SWIPE_X;

  command_buffer[7].type = COMMAND_TWANG;

  // for (int i = 0; i < 30; i++)
  // {
  //   command_buffer[i + 1].type = COMMAND_SHOW_NUMBER;
  //   command_buffer[i + 1].data[0] = i;
  //   command_buffer[i + 1].data[1] = i + 1;
  // }

  // command_buffer[1].type = COMMAND_DEBUG;
  // command_buffer[0].data[0] = 29;
}

void Dode::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index < EDGE_COUNT)
  {
    edge_leds[edges[strip_index][0]][edges[strip_index][1] + led] = color;
  }
  else
  {
    for (int i = 0; i < EDGE_COUNT; i++)
    {
      if (led < strip_length(i))
      {
        edge_leds[edges[i][0]][edges[i][1] + led] = color;
      }
    }
  }
}

CRGB Dode::get_led(uint8_t strip_index, int led)
{
  if (strip_index < EDGE_COUNT)
  {
    return edge_leds[edges[strip_index][0]][edges[strip_index][1] + led];
  }
  else
  {
    return edge_leds[edges[0][0]][edges[0][1] + led];
  }
}

void Dode::fade_led(uint8_t strip_index, int led, CRGB target, float amount)
{
  // todo: remove strip_length lookup
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount * 255.0);
    set_led(strip_index, led, faded);
  }
}

void Dode::set_led_mirrored_middle(uint8_t edge_index, uint8_t led_index, CRGB color)
{
  if (led_index <= strip_length(edge_index) / 2 - 1)
  {
    set_led(edge_index, strip_length(edge_index) / 2 - 1 - led_index, color);
    set_led(edge_index, strip_length(edge_index) / 2 + led_index, color);
  }
}

void Dode::all_white()
{
  for (int i = 0; i < EDGE_MAX_LENGTH; i++)
  {
    set_led(EDGE_COUNT, i, CRGB(255, 255, 255));
  }
}

uint8_t Dode::random_strip(uint8_t strip_index)
{
  if (strip_index < EDGE_COUNT)
  {
    return strip_index;
  }
  else
  {
    return random8(0, EDGE_COUNT);
  }
}

uint16_t Dode::strip_length(uint8_t strip_index)
{
  if (strip_index < EDGE_COUNT)
  {
    return edges[strip_index][2];
  }
  else
  {
    return EDGE_MAX_LENGTH;
  }
}