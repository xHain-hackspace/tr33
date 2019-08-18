#include <Commands.h>

Leds *leds;

Command command_buffer[COMMAND_BUFFER_SIZE];
CRGBPalette256 currentPalette = Rainbow_gp;
uint8_t currentMode = MODE_COMMANDS;

void Commands::init(Leds *leds_instance)
{
  leds = leds_instance;
  FastLED.setCorrection(TypicalLEDStrip);
  leds->init();
};

void Commands::process(char *command_bin)
{
  Command command = *(Command *)command_bin;

  if (command.type > 100)
  {
    switch (command.type)
    {
    case EVENT_GRAVITY_ADD_BALL:
      leds->gravity_event();
      break;
    case EVENT_UPDATE_SETTINGS:
      update_settings(command.data);
      break;
    case EVENT_PIXEL:
      pixel(leds, command.data);
      break;
    case EVENT_PIXEL_RGB:
      pixel_rgb(leds, command.data);
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
        single_color(leds, command_buffer[i].data);
        break;
      case COMMAND_WHITE:
        leds->all_white();
        break;
      case COMMAND_RAINBOW_SINE:
        leds->rainbow_sine(command_buffer[i].data);
        break;
      case COMMAND_PING_PONG:
        leds->ping_pong(command_buffer[i].data);
        break;
      case COMMAND_GRAVITY:
        leds->gravity(command_buffer[i].data);
        break;
      case COMMAND_SPARKLE:
        sparkle(leds, command_buffer[i].data);
        break;
      case COMMAND_SHOW_NUMBER:
        show_number(leds, command_buffer[i].data);
        break;
      case COMMAND_RAIN:
        leds->rain(command_buffer[i].data);
        break;
      case COMMAND_BEATS:
        leds->beats(command_buffer[i].data);
        break;
      case COMMAND_MAPPED_SWIPE:
        leds->mapped_swipe(command_buffer[i].data);
        break;
      case COMMAND_MAPPED_SHAPE:
        leds->mapped_shape(command_buffer[i].data);
        break;
      case COMMAND_KALEIDOSCOPE:
        leds->kaleidoscope(command_buffer[i].data);
        break;
      case COMMAND_RANDOM_WALK:
        leds->random_walk(command_buffer[i].data);
        break;
      case COMMAND_DEBUG:
        leds->debug(command_buffer[i].data);
        break;
      default:
        break;
      }
    }
  }

  FastLED.show();
}