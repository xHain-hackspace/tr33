#include <Commands.h>

Command command_buffer[COMMAND_BUFFER_SIZE];
CRGBPalette256 currentPalette = Rainbow_gp;
uint8_t currentMode = MODE_COMMANDS;

void Commands::init(LedStructure *init_leds)
{
  leds = init_leds;
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
      gravity_event(leds, command.data);
      break;
    case EVENT_UPDATE_SETTINGS:
      update_settings(command.data);
      break;
    case EVENT_PIXEL:
      pixel(leds, command.data);
      FastLED.show();
      break;
    case EVENT_PIXEL_RGB:
      pixel_rgb(leds, command.data);
      FastLED.show();
      break;
    case EVENT_JOYSTICK:
      leds->joystick(command.data);
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

void Commands::render_commands()
{
  for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
  {
    switch (command_buffer[i].type)
    {
    case COMMAND_SINGLE_COLOR:
      single_color(leds, command_buffer[i].data);
      break;
    case COMMAND_WHITE:
      white(leds, command_buffer[i].data);
      break;
    case COMMAND_RAINBOW_SINE:
      rainbow_sine(leds, command_buffer[i].data);
      break;
    case COMMAND_PING_PONG:
      ping_pong(leds, command_buffer[i].data);
      break;
    case COMMAND_GRAVITY:
      gravity(leds, command_buffer[i].data);
      break;
    case COMMAND_SPARKLE:
      sparkle(leds, command_buffer[i].data);
      break;
    case COMMAND_FLICKER_SPARKLE:
      flicker_sparkle(leds, command_buffer[i].data);
      break;
    case COMMAND_SHOW_NUMBER:
      show_number(leds, command_buffer[i].data);
      break;
    case COMMAND_RAIN:
      rain(leds, command_buffer[i].data);
      break;
    case COMMAND_BEATS:
      leds->beats(command_buffer[i].data);
      break;
    case COMMAND_MAPPED_SLOPE:
      mapped_slope(leds, command_buffer[i].data);
      break;
    case COMMAND_MAPPED_SHAPE:
      mapped_shape(leds, command_buffer[i].data);
      break;
    case COMMAND_MAPPED_TRIANGLE:
      mapped_triangle(leds, command_buffer[i].data);
      break;
    case COMMAND_MAPPED_PARTICLES:
      mapped_particles(leds, command_buffer[i].data);
      break;
    case COMMAND_KALEIDOSCOPE:
      kaleidoscope(leds, command_buffer[i].data);
      break;
    case COMMAND_RANDOM_WALK:
      leds->random_walk(command_buffer[i].data);
      break;
    case COMMAND_DEBUG:
      leds->debug(command_buffer[i].data);
      break;
    case COMMAND_FIREWORKS:
      leds->fireworks(command_buffer[i].data);
      break;
    case COMMAND_ROTATING_SECTORS:
      leds->rotating_sectors(command_buffer[i].data);
      break;
    case COMMAND_RENDER:
      render(leds, command_buffer[i].data);
      break;
    case COMMAND_ROTATING_PLANE:
      leds->rotating_plane(command_buffer[i].data);
      break;
    case COMMAND_TWANG:
      leds->twang(command_buffer[i].data);
      break;
    default:
      break;
    }
  }
}

void Commands::run()
{
  switch (currentMode)
  {
  // case MODE_ARTNET:
  //   artnet.read();
  //   break;
  case MODE_STREAM:
    // FastLED.show();
    break;
  default:
    // Serial.println("RUN");
    FastLED.clearData();
    render_commands();
    FastLED.show();
    break;
  }
}

void Commands::artnet_sync_callback()
{
  FastLED.show();
}
