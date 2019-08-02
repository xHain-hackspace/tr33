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
  command_buffer[0].type = COMMAND_SINGLE_COLOR;
  command_buffer[0].data[0] = HUE_BLUE;
  command_buffer[0].data[1] = 255;
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
  case COMMAND_RANDOM_WALK:
    random_walk(command->data);
  }
}
