#include <Dode.h>
#include <Commands.h>

CRGB edge_leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

Dode::Dode(void)
{
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_0>(edge_leds[0], HW_STRIP_PIXEL_COUNT);
}

void Dode::init()
{
  command_buffer[0].type = COMMAND_SINGLE_COLOR;
  command_buffer[0].data[0] = HUE_PURPLE;
  command_buffer[0].data[1] = 10;

  // command_buffer[1].type = COMMAND_KALEIDOSCOPE;
  // command_buffer[1].data[0] = HUE_PINK;
  // command_buffer[1].data[1] = 255;
  // command_buffer[1].data[2] = 20;

  command_buffer[1].type = COMMAND_RANDOM_WALK;
  command_buffer[1].data[0] = HUE_RED;
  command_buffer[1].data[1] = 5;
  command_buffer[1].data[2] = 100;
  command_buffer[1].data[3] = 3;
}

void Dode::process_event(Command *command){

};

void Dode::process_command(Command *command)
{
  switch (command->type)
  {
  case COMMAND_SINGLE_COLOR:
    single_color(command->data);
    break;
  case COMMAND_KALEIDOSCOPE:
    kaleidoscope(command->data);
    break;
  case COMMAND_RANDOM_WALK:
    random_walk(command->data);
    break;
  }
}
