#include <Dode.h>
#include <Commands.h>

Commands commands_2;
CRGB leds[HW_STRIP_COUNT][HW_STRIP_PIXEL_COUNT];

Dode::Dode(void)
{
  FastLED.addLeds<NEOPIXEL, HW_STRIP_PIN_0>(leds[0], HW_STRIP_PIXEL_COUNT);
}

void Dode::init()
{
  // command_buffer[0].type = COMMAND_SINGLE_COLOR;
  // command_buffer[0].data[0] = HUE_ORANGE;
  // command_buffer[0].data[1] = 10;

  // command_buffer[0].type = COMMAND_KALEIDOSCOPE;
  // command_buffer[0].data[0] = HUE_PINK;
  // command_buffer[0].data[1] = 255;
  // command_buffer[0].data[2] = 20;

  command_buffer[1].type = COMMAND_RANDOM_WALK;
  command_buffer[1].data[0] = HUE_RED;
  command_buffer[1].data[1] = 5;
  command_buffer[1].data[2] = 100;
  command_buffer[1].data[3] = 10;
}

void Dode::process_event(Command *command)
{
  switch (command->type)
  {
  case EVENT_UPDATE_SETTINGS:
    commands_2.update_settings(command->data);
    break;
  };
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
