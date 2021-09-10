#include <Arduino.h>
#include <Commands.h>
#include <Modifiers.h>
#include <Beats.h>

void Modifiers::apply_all(CommandParams *cmd)
{
  int value = 0;

  for (int i = 0; i < cmd->modifiers_count; i++)
  {
    if (cmd->modifiers[i].period_100ms > 0)
    {
      switch (cmd->modifiers[i].field_index)
      {
      case 0:
        apply(cmd->modifiers[i], &cmd->brightness, cmd->index);
        break;
      case 1:
        apply(cmd->modifiers[i], &value, cmd->index);
        cmd->color_palette = static_cast<ColorPalette>(value);
        break;
      case 2:
        apply(cmd->modifiers[i], &cmd->strip_index, cmd->index);
        break;
      default:
        apply(cmd->modifiers[i], value_from_type_param(cmd, cmd->modifiers[i].field_index), cmd->index);
        break;
      }
    }
  }
}

uint16_t linear(uint16_t in)
{
  if (in > UINT16_MAX / 2)
  {
    in = UINT16_MAX - in;
  }
  return in * 2;
}

uint16_t sawtooth(uint32_t period_ms, uint32_t offset_ms)
{
  return (Commands::synced_millis() + offset_ms % period_ms) * UINT16_MAX / period_ms;
}

void Modifiers::apply(Modifier modifier, int *value, uint8_t command_index)
{
  switch (modifier.movement_type)
  {
  case MovementType_LINEAR:
  case MovementType_SINE:
  case MovementType_QUADRATIC:
  case MovementType_SAWTOOTH:
  case MovementType_SAWTOOTH_REVERSE:
    apply_envelope(modifier, value);
    break;
  case MovementType_RANDOM:
  case MovementType_RANDOM_TRANSITIONS:
    apply_random(modifier, value, command_index);
    break;
  case MovementType_BEATS_1:
    apply_beat(modifier, value, 0);
    break;
  case MovementType_BEATS_2:
    apply_beat(modifier, value, 1);
    break;
  case MovementType_BEATS_3:
    apply_beat(modifier, value, 2);
    break;
  case MovementType_BEATS_4:
    apply_beat(modifier, value, 3);
    break;
  case MovementType_BEATS_5:
    apply_beat(modifier, value, 4);
    break;
  case MovementType_BEATS_6:
    apply_beat(modifier, value, 5);
    break;
  case MovementType_BEATS_7:
    apply_beat(modifier, value, 6);
    break;
  case MovementType_BEATS_ALL:
    apply_beat(modifier, value, 7);
    break;
  }
}

void Modifiers::apply_envelope(Modifier modifier, int *value)
{
  uint16_t beat = sawtooth(modifier.period_100ms * 100, modifier.offset_100ms * 100);
  uint16_t rangewidth = modifier.max - modifier.min;
  uint16_t applied;

  switch (modifier.movement_type)
  {
  case MovementType_LINEAR:
    applied = linear(beat);
    break;
  case MovementType_SINE:
    applied = sin16(beat) + UINT16_MAX / 2;
    break;
  case MovementType_QUADRATIC:
    applied = ease16InOutQuad(linear(beat));
    break;
  case MovementType_SAWTOOTH:
    applied = beat;
  case MovementType_SAWTOOTH_REVERSE:
    applied = UINT16_MAX - beat;
    break;
  }

  uint16_t scaled = scale16(applied, rangewidth);
  *value = modifier.min + scaled;
}

uint16_t easeOutQuart(uint16_t start, uint16_t end, float progress)
{
  float eased = 1 - powf(1 - progress, 4);
  return start + uint16_t(float(end - start) * eased);
}

uint32_t last_random_ms[COMMAND_COUNT][MAX_MODIFIERS];
uint32_t last_random_value[COMMAND_COUNT][MAX_MODIFIERS];
uint32_t next_random_value[COMMAND_COUNT][MAX_MODIFIERS];

void Modifiers::apply_random(Modifier modifier, int *value, uint8_t command_index)
{
  uint32_t now_ms = (Commands::synced_millis() - modifier.offset_100ms * 100);
  uint32_t since_last_random = now_ms - last_random_ms[command_index][modifier.field_index];
  uint32_t period_ms = modifier.period_100ms * 100;

  if (modifier.movement_type == MovementType_RANDOM)
  {
    *value = last_random_value[command_index][modifier.field_index];
  }
  else if (modifier.movement_type == MovementType_RANDOM_TRANSITIONS)
  {
    float progress = float(since_last_random) / float(period_ms);
    *value = easeOutQuart(last_random_value[command_index][modifier.field_index], next_random_value[command_index][modifier.field_index], progress);
  }

  if (since_last_random > period_ms)
  {
    last_random_ms[command_index][modifier.field_index] = now_ms;
    last_random_value[command_index][modifier.field_index] = next_random_value[command_index][modifier.field_index];
    next_random_value[command_index][modifier.field_index] = random16(modifier.min, modifier.max);
  }
}

void Modifiers::apply_beat(Modifier modifier, int *value, uint8_t band)
{
  uint16_t rangewidth = modifier.max - modifier.min;
  uint16_t scaled = scale16(Beats::read_band(band), rangewidth);
  *value = modifier.min + scaled;
}
