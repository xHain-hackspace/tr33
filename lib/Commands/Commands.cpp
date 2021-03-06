#include <Commands.h>
#include <Modifiers.h>
#include <pb_decode.h>

CommandParams commands[COMMAND_COUNT];
int64_t Commands::millis_offset = 0;

void Commands::init(LedStructure *init_leds)
{
  leds = init_leds;
  FastLED.setCorrection(TypicalLEDStrip);
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    commands[i] = CommandParams_init_zero;
  }
  leds->init();

  load_palettes();
}

void Commands::handle_message(WireMessage msg)
{
  switch (msg.which_message)
  {
  case WireMessage_command_params_tag:
    handle_command(msg.message.command_params);
    break;
  case WireMessage_time_sync_tag:
    millis_offset = msg.message.time_sync.millis - millis();
    break;
  }
}

void Commands::handle_command(CommandParams cmd)
{
  if (cmd.index < COMMAND_COUNT)
  {
    commands[cmd.index] = cmd;
  }
}

void Commands::process(uint8_t *buffer, int bytes)
{
  pb_istream_t stream = pb_istream_from_buffer(buffer, bytes);
  CommandParams command = CommandParams_init_default;
  bool status = pb_decode(&stream, CommandParams_fields, &command);

  if (!status)
  {
    Serial.printf("Protobuf decoding failed: %s\n", PB_GET_ERROR(&stream));
  }
  else
  {
    // process(command);
  }
}

void Commands::render_commands()
{
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    if (commands[i].enabled)
    {
      Modifiers::apply_all(&commands[i]);

      switch (commands[i].which_type_params)
      {
      case CommandParams_white_tag:
        white(leds, commands[i]);
        break;
      case CommandParams_single_color_tag:
        single_color(leds, commands[i]);
        break;
      case CommandParams_pixel_tag:
        pixel(leds, commands[i]);
        break;
      case CommandParams_pixel_rgb_tag:
        pixel_rgb(leds, commands[i]);
        break;
      case CommandParams_rainbow_tag:
        rainbow(leds, commands[i]);
        break;
      case CommandParams_sparkle_tag:
        sparkle(leds, commands[i]);
        break;
      case CommandParams_flicker_sparkle_tag:
        flicker_sparkle(leds, commands[i]);
        break;
      // case CommandParams_show_number_tag:
      //   show_number(leds, commands[i]);
      //   break;
      case CommandParams_rain_tag:
        rain(leds, commands[i]);
        break;
      case CommandParams_render_tag:
        render(leds, commands[i]);
        break;
      case CommandParams_ping_pong_tag:
        ping_pong(leds, commands[i]);
        break;
      case CommandParams_kaleidoscope_tag:
        kaleidoscope(leds, commands[i]);
        break;
      case CommandParams_mapped_slope_tag:
        mapped_slope(leds, commands[i]);
        break;
      case CommandParams_mapped_shape_tag:
        mapped_shape(leds, commands[i]);
        break;
      case CommandParams_mapped_triangle_tag:
        mapped_triangle(leds, commands[i]);
        break;
      case CommandParams_mapped_particles_tag:
        mapped_particles(leds, commands[i]);
        break;
      case CommandParams_mapped_ping_pong_tag:
        mapped_ping_pong(leds, commands[i]);
        break;
      case CommandParams_gravity_tag:
        gravity(leds, commands[i]);
        break;
      case CommandParams_twang_tag:
        twang(leds);
        break;
      }
    }
  }
}

uint16_t fps;

void Commands::run()
{
  // switch (currentMode)
  // {
  // case MODE_ARTNET:
  //   artnet.read();
  //   break;
  // case MODE_STREAM:
  //   // FastLED.show();
  //   break;
  // default:
  // Serial.println("RUN");
  FastLED.clearData();
  render_commands();
  FastLED.show();
  // break;
  // }

#ifdef SEND_FPS
  if (FastLED.getFPS() != fps)
  {
    fps = FastLED.getFPS();
    Serial.printf("FPS: %i\n", fps);
  }
#endif
}

uint64_t Commands::synced_millis()
{
  return millis() + millis_offset;
}

void Commands::artnet_sync_callback()
{
  FastLED.show();
}
