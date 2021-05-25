#include <Commands.h>
#include <Modifiers.h>
#include <pb_decode.h>

Command command_buffer[COMMAND_BUFFER_SIZE];
CommandParams commands[COMMAND_COUNT];
CRGBPalette256 currentPalette = Rainbow_gp;
uint8_t currentMode = MODE_COMMANDS;

void Commands::init(LedStructure *init_leds)
{
  leds = init_leds;
  FastLED.setCorrection(TypicalLEDStrip);
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    commands[i] = CommandParams_init_zero;
  }
  leds->init();
};

void Commands::process(CommandParams cmd)
{
  if (cmd.index < COMMAND_BUFFER_SIZE)
  {
    commands[cmd.index] = cmd;
  }
}

void Commands::process(uint8_t *command_bin, int bytes)
{

  // Command command = *(Command *)command_bin;

  // if (command.type > 100)
  // {
  //   switch (command.type)
  //   {
  //   case EVENT_GRAVITY_ADD_BALL:
  //     gravity_event(leds, command.data);
  //     break;
  //   case EVENT_UPDATE_SETTINGS:
  //     update_settings(command.data);
  //     break;
  //   case EVENT_PIXEL:
  //     pixel(leds, command.data);
  //     FastLED.show();
  //     break;
  //   case EVENT_PIXEL_RGB:
  //     pixel_rgb(leds, command.data);
  //     FastLED.show();
  //     break;
  //   case EVENT_TWANG_JOYSTICK:
  //     twang_joystick(command.data);
  //     break;
  //   case EVENT_MODIFIER_UPDATE:
  //     Modifiers::update(command_bin + 2);
  //   default:
  //     break;
  //   };
  // }
  // else
  // {
  pb_istream_t stream = pb_istream_from_buffer(command_bin, bytes);
  CommandParams command = CommandParams_init_default;
  bool status = pb_decode(&stream, CommandParams_fields, &command);

  if (!status)
  {
    Serial.printf("Protobuf decoding failed: %s\n", PB_GET_ERROR(&stream));
  }
  else
  {
    process(command);
  }
}

void Commands::render_commands()
{
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    if (commands[i].enabled)
    {
      switch (commands[i].which_type_params)
      {
      case CommandParams_white_tag:
        white(leds, commands[i]);
        break;
      case CommandParams_single_color_tag:
        single_color(leds, commands[i]);
        break;
      case CommandParams_rainbow_tag:
        rainbow(leds, commands[i]);
      }
    }
  }

  // Modifiers::apply(command_buffer);

  // for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
  // {
  //   switch (command_buffer[i].type)
  //   {
  //   case COMMAND_SINGLE_COLOR:
  //     single_color(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_WHITE:
  //     white(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_RAINBOW_SINE:
  //     rainbow_sine(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_PING_PONG:
  //     ping_pong(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_GRAVITY:
  //     gravity(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_SPARKLE:
  //     sparkle(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_FLICKER_SPARKLE:
  //     flicker_sparkle(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_SHOW_NUMBER:
  //     show_number(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_RAIN:
  //     rain(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_BEATS:
  //     leds->beats(command_buffer[i].data);
  //     break;
  //   case COMMAND_MAPPED_SLOPE:
  //     mapped_slope(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_MAPPED_SHAPE:
  //     mapped_shape(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_MAPPED_TRIANGLE:
  //     mapped_triangle(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_MAPPED_PARTICLES:
  //     mapped_particles(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_KALEIDOSCOPE:
  //     kaleidoscope(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_RANDOM_WALK:
  //     leds->random_walk(command_buffer[i].data);
  //     break;
  //   case COMMAND_DEBUG:
  //     leds->debug(command_buffer[i].data);
  //     break;
  //   case COMMAND_FIREWORKS:
  //     leds->fireworks(command_buffer[i].data);
  //     break;
  //   case COMMAND_ROTATING_SECTORS:
  //     leds->rotating_sectors(command_buffer[i].data);
  //     break;
  //   case COMMAND_RENDER:
  //     render(leds, command_buffer[i].data);
  //     break;
  //   case COMMAND_ROTATING_PLANE:
  //     leds->rotating_plane(command_buffer[i].data);
  //     break;
  //   case COMMAND_TWANG:
  //     twang(leds);
  //     break;
  //   default:
  //     break;
  //   }
  // }
}

uint16_t fps;

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

#ifdef SEND_FPS
  if (FastLED.getFPS() != fps)
  {
    fps = FastLED.getFPS();
    Serial.printf("FPS: %i\n", fps);
  }
#endif
}

void Commands::artnet_sync_callback()
{
  FastLED.show();
}
