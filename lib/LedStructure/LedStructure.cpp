#include <Commands.h>
#include <LedStructure.h>
#include <command_schemas.pb.h>
// #include <Modifiers.h>

CRGB LedStructure::leds[STRIP_COUNT][STRIP_PIXEL_COUNT];

void LedStructure::init()
{
  SingleColor single = SingleColor_init_default;
  single.color = HUE_BLUE;
  commands[0] = CommandParams_init_default;
  commands[0].which_type_params = CommandParams_single_color_tag;
  commands[0].type_params.single_color = single;

  // PingPong ping_pong = PingPong_init_default;
  // ping_pong.color = HUE_RED;
  // commands[1] = CommandParams_init_default;
  // commands[1].which_type_params = CommandParams_ping_pong_tag;
  // commands[1].type_params.ping_pong = ping_pong;
}

//
// -- Set leds ----------------------------------------
//

void LedStructure::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index < STRIP_COUNT)
  {
    leds[strip_index][led] = color;
  }
  else
  {
    for (int i = 0; i < STRIP_COUNT; i++)
    {
      leds[i][led] = color;
    }
  }
}

CRGB LedStructure::get_led(uint8_t strip_index, int led)
{
  if (strip_index < STRIP_COUNT)
  {
    return leds[strip_index][led];
  }
  else
  {
    return leds[0][led];
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

void LedStructure::fade_led(CommandParams cmd, int led, CRGB target)
{
  if (led >= 0 && led < strip_length(cmd.strip_index))
  {
    CRGB current = get_led(cmd.strip_index, led);
    CRGB faded = blend(current, target, cmd.brightness);
    set_led(cmd.strip_index, led, faded);
  }
}

uint16_t LedStructure::strip_length(uint8_t strip_index)
{
  return STRIP_PIXEL_COUNT;
}

uint16_t LedStructure::pixel_count(uint8_t strip_index)
{
  if (strip_index < STRIP_COUNT)
  {
    return STRIP_PIXEL_COUNT;
  }
  else if (strip_index == STRIP_INDEX_ALL)
  {
    return STRIP_PIXEL_COUNT * STRIP_COUNT;
  }
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

void LedStructure::beats(uint8_t *data)
{
  return;
}

void LedStructure::random_walk(uint8_t *data)
{
  return;
}

void LedStructure::debug(uint8_t *data)
{
  return;
}

void LedStructure::fireworks(uint8_t *data)
{
  return;
}

void LedStructure::rotating_sectors(uint8_t *data)
{
  return;
}

void LedStructure::rotating_plane(uint8_t *data)
{
  return;
}

void LedStructure::joystick(uint8_t *data)
{
  return;
}

void LedStructure::write_info()
{
  Serial.println("Generic Led Structure");
  return;
}
