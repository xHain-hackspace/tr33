#include <Commands.h>
#include <Tr33.h>
#include <Dode.h>

Tr33 leds = Tr33();

// Dode leds = Dode();

Command command_buffer[COMMAND_BUFFER_SIZE];
CRGBPalette256 currentPalette = Rainbow_gp;
uint8_t currentMode = MODE_COMMANDS;

void Commands::init()
{
  FastLED.setCorrection(TypicalLEDStrip);
  leds.init();
};

void Commands::process(char *command_bin)
{
  Command command = *(Command *)command_bin;

  if (command.type > 100)
  {
    switch (command.type)
    {
    case EVENT_GRAVITY_ADD_BALL:
      leds.gravity_event();
      break;
    case EVENT_UPDATE_SETTINGS:
      update_settings(command.data);
      break;
    case EVENT_PIXEL:
      leds.pixel(command.data);
      break;
    case EVENT_PIXEL_RGB:
      leds.pixel_rgb(command.data);
      break;
    default:
      break;
    };
  }
  else
  {
    if (command.index < COMMAND_BUFFER_SIZE)
    {
      command_buffer[command.index] = command;
    }
  }
}

void Commands::run()
{
  bool run_commands;

  switch (currentMode)
  {
  case MODE_STREAM:
    run_commands = false;
    break;
  default:
    run_commands = true;
    FastLED.clearData();
    break;
  }

  if (run_commands)
  {
    for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
    {
      switch (command_buffer[i].type)
      {
      case COMMAND_SINGLE_COLOR:
        leds.single_color(command_buffer[i].data);
        break;
      case COMMAND_WHITE:
        leds.all_white();
        break;
      case COMMAND_RAINBOW_SINE:
        leds.rainbow_sine(command_buffer[i].data);
        break;
      case COMMAND_PING_PONG:
        leds.ping_pong(command_buffer[i].data);
        break;
      case COMMAND_GRAVITY:
        leds.gravity(command_buffer[i].data);
        break;
      case COMMAND_SPARKLE:
        sparkle(&leds, command_buffer[i].data);
        break;
      case COMMAND_SHOW_NUMBER:
        leds.show_number(command_buffer[i].data);
        break;
      case COMMAND_RAIN:
        leds.rain(command_buffer[i].data);
        break;
      case COMMAND_BEATS:
        leds.beats(command_buffer[i].data);
        break;
      case COMMAND_MAPPED_SWIPE:
        leds.mapped_swipe(command_buffer[i].data);
        break;
      case COMMAND_MAPPED_SHAPE:
        leds.mapped_shape(command_buffer[i].data);
        break;

      default:
        break;
      }
    }
  }

  FastLED.show();
}