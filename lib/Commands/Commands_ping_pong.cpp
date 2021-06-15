#include <Commands.h>
#include <Modifiers.h>

int last_value[COMMAND_COUNT];

void Commands::ping_pong(LedStructure *leds, CommandParams cmd)
{
  PingPong ping_pong = cmd.type_params.ping_pong;

  float brightness = float(cmd.brightness) / 255;
  // float max_height = float(ping_pong.max_height) / 255;

  CRGB color = color_from_palette(cmd, ping_pong.color);

  int value = 0;
  Modifier modifier = Modifier_init_default;
  modifier.field_index = MAX_MODIFIERS;
  modifier.movement_type = ping_pong.movement;
  modifier.max = 65535;
  modifier.period_100ms = max(1, ping_pong.period_100ms);

  Modifiers::apply(modifier, &value, cmd.index);

  bool direction;

  switch (ping_pong.movement)
  {
  case MovementType_SAWTOOTH:
    direction = false;
    break;
  case MovementType_SAWTOOTH_REVERSE:
    direction = true;
    break;
  default:
    direction = value > last_value[cmd.index];
  }
  last_value[cmd.index] = value;

  for (int i = 0; i < max(1, ping_pong.count); i++)
  {
    float position = float((value + (i * 65535) / max(1, ping_pong.count)) % 65535) / 65535 * float(leds->strip_length(cmd.strip_index)); //* max_height;
    render(leds, ping_pong.shape, cmd.strip_index, position, ping_pong.width, color, brightness, direction);
  }
}