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

  bool direction = value > last_value[cmd.index];
  bool bounce = false;
  bool wrap = false;

  switch (ping_pong.movement)
  {
  case MovementType_LINEAR:
    bounce = true;
    break;
  case MovementType_SINE:
    bounce = true;
    break;
  case MovementType_QUADRATIC:
    bounce = true;
    break;
  case MovementType_SAWTOOTH:
    direction = false;
    wrap = true;
    break;
  case MovementType_SAWTOOTH_REVERSE:
    wrap = true;
    direction = true;
    break;
  case MovementType_RANDOM:
    break;
  case MovementType_RANDOM_TRANSITIONS:
    break;
  case MovementType_BEATS_1:
    break;
  case MovementType_BEATS_2:
    break;
  case MovementType_BEATS_3:
    break;
  case MovementType_BEATS_4:
    break;
  case MovementType_BEATS_5:
    break;
  case MovementType_BEATS_6:
    break;
  case MovementType_BEATS_7:
    break;
  case MovementType_BEATS_ALL:
    break;
  case MovementType_JOYSTICK_X:
    break;
  case MovementType_JOYSTICK_Y:
    break;
  }
  last_value[cmd.index] = value;

  for (int i = 0; i < max(1, ping_pong.count); i++)
  {
    float position = float((value + (i * 65535) / max(1, ping_pong.count)) % 65535) / 65535 * float(leds->strip_length(cmd.strip_index)); //* max_height;
    render(leds, ping_pong.shape, cmd.strip_index, position, ping_pong.width, color, brightness, direction, bounce, wrap);
  }
}