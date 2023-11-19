#include <Commands.h>
#include <Modifiers.h>
#include <pb_decode.h>
#include <Network.h>

CommandParams commands[COMMAND_COUNT];
EffectItem effect_items[EFFECT_ITEM_COUNT];

int64_t Commands::millis_offset = 0;
uint8_t Commands::joystick_x = 0;
uint8_t Commands::joystick_y = 0;
boolean Commands::joystick_button = false;

FirmwareConfig Commands::firmware_config = FirmwareConfig_init_default;

// debug
uint32_t debug_interval = 1000;
uint32_t last_debug = 0;
long durations[6];

void Commands::init(LedStructure *init_leds)
{
  leds = init_leds;
  FastLED.setCorrection(TypicalLEDStrip);
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    commands[i] = (CommandParams)CommandParams_init_zero;
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
  case WireMessage_joystick_event_tag:
    joystick_x = msg.message.joystick_event.x;
    joystick_y = msg.message.joystick_event.y;
    joystick_button = msg.message.joystick_event.button1;
    break;
  case WireMessage_firmware_config_tag:
    if (msg.message.firmware_config.calibration_fastled != firmware_config.calibration_fastled || msg.message.firmware_config.calibration_custom != firmware_config.calibration_custom)
    {
      if (firmware_config.calibration_fastled)
      {
        FastLED.setCorrection(TypicalLEDStrip); /* 255, 176, 240 */
      }
      else if (firmware_config.calibration_custom)
      {
        FastLED.setCorrection(CRGB(255, 168, 242)); // old CRGB(255, 246, 102)
      }
      else
      {
        FastLED.setCorrection(UncorrectedColor);
      }
    }

    firmware_config = msg.message.firmware_config;

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
  if (millis() - last_debug > debug_interval)
  {
    send_debug = true;
    durations[0] = millis();
  }

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
      case CommandParams_calibrate_tag:
        calibrate(leds, commands[i]);
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
      case CommandParams_fairy_light_tag:
        fairy_light(leds, commands[i]);
        break;
      case CommandParams_pixel_func_tag:
        if (send_debug)
        {
          durations[1] = millis();
        }
        pixel_func(leds, commands[i]);
        if (send_debug)
        {
          durations[2] = millis();
        }
        break;
      case CommandParams_debug_tag:
        debug(leds, commands[i]);
        break;
      }
    }
  }

  if (send_debug)
  {
    durations[3] = millis();
  }

  if (send_debug)
  {
    last_debug = millis();
    send_debug = false;
    Network::remote_log("Durations. PixelFun: " + String(durations[2] - durations[1]) + " Total: " + String(durations[3] - durations[0]));
  }
}

uint16_t fps;

void Commands::run()
{
  FastLED.clearData();
  render_commands();
  FastLED.show();
}

uint64_t Commands::synced_millis()
{
  return millis() + millis_offset;
}

void Commands::write_hashes(TargetMetrics *target_metrics)
{
  for (int i = 0; i < COMMAND_COUNT; i++)
  {
    if (commands[i].enabled)
    {
      memcpy(&target_metrics->hashes[i], &commands[i].hash, sizeof(TargetMetrics_hashes_t));
    }
    else
    {
      memset(&target_metrics->hashes[i], 0, sizeof(TargetMetrics_hashes_t));
    }
  }

  target_metrics->hashes_count = COMMAND_COUNT;

  return;
};
