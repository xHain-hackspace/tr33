#include <Modifiers.h>

Modifier Modifiers::modifiers[];

void Modifiers::test()
{
  modifiers[1].command_index = 1;
  modifiers[1].data_index = 4;
  modifiers[1].data_length = 2;
  modifiers[1].type = MODIFIER_LINEAR;
  modifiers[1].beats_per_minute = 4 * 256;
  modifiers[1].max = 255;
}

void Modifiers::update(uint8_t *data)
{
  for (int i = 0; i < MODIFIER_COUNT; i++)
  {
    modifiers[i].type = data[i * COMMAND_DATA_SIZE + 0];
    modifiers[i].command_index = data[i * COMMAND_DATA_SIZE + 1];
    modifiers[i].data_index = data[i * COMMAND_DATA_SIZE + 2];
    modifiers[i].data_length = data[i * COMMAND_DATA_SIZE + 3];
    modifiers[i].beats_per_minute = data[i * COMMAND_DATA_SIZE + 4] << 8 | data[i * COMMAND_DATA_SIZE + 5];
    modifiers[i].offset_100ms = data[i * COMMAND_DATA_SIZE + 6] << 8 | data[i * COMMAND_DATA_SIZE + 7];
    modifiers[i].max = data[i * COMMAND_DATA_SIZE + 8];
    modifiers[i].min = data[i * COMMAND_DATA_SIZE + 9];
  }
}

void Modifiers::apply(Command command_buffer[])
{
  for (uint8_t i = 0; i < MODIFIER_COUNT; i++)
  {
    switch (modifiers[i].type)
    {
    case MODIFIER_DISABLED:
      break;
    case MODIFIER_LINEAR:
    case MODIFIER_SINE:
    case MODIFIER_QUADRATIC:
    case MODIFIER_CUBIC:
    case MODIFIER_SAWTOOTH:
    case MODIFIER_SAWTOOTH_REVERSE:
      apply_envelope(&modifiers[i], command_buffer);
      break;
    case MODIFIER_RANDOM_TRANSITIONS:
    case MODIFIER_RANDOM:
      apply_random(&modifiers[i], command_buffer);
      break;
      // case MODIFIER_BOUNCE 7:
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

void set_value(Modifier *modifier, uint16_t value, Command command_buffer[])
{
  if (modifier->data_length == 2)
  {
    command_buffer[modifier->command_index].data[modifier->data_index] = value >> 8;
    command_buffer[modifier->command_index].data[modifier->data_index + 1] = value;
  }
  else
  {
    command_buffer[modifier->command_index].data[modifier->data_index] = value >> 8;
  }
}

void Modifiers::apply_envelope(Modifier *modifier, Command command_buffer[])
{
  uint32_t offset_ms = uint32_t(modifier->offset_100ms) * 100;
  uint16_t beat = beat88(modifier->beats_per_minute, offset_ms);
  uint16_t rangewidth = modifier->max * 256 - modifier->min * 256;
  uint16_t applied;

  switch (modifier->type)
  {
  case MODIFIER_LINEAR:
    applied = linear(beat);
    break;
  case MODIFIER_SINE:
    applied = sin16(beat) + UINT16_MAX / 2;
    break;
  case MODIFIER_QUADRATIC:
  case MODIFIER_CUBIC: // todo
    applied = ease16InOutQuad(linear(beat));
    break;
  case MODIFIER_SAWTOOTH:
    applied = beat;
  case MODIFIER_SAWTOOTH_REVERSE:
    applied = UINT16_MAX - beat;
    break;
  }

  uint16_t scaled = scale16(applied, rangewidth);
  uint16_t result = modifier->min * 256 + scaled;
  set_value(modifier, result, command_buffer);
}

uint16_t easeOutQuart(uint16_t start, uint16_t end, float progress)
{
  float eased = 1 - powf(1 - progress, 4);
  return start + uint16_t(float(end - start) * eased);
}

void Modifiers::apply_random(Modifier *modifier, Command command_buffer[])
{
  uint32_t offset_ms = uint32_t(modifier->offset_100ms) * 100;
  uint32_t now_ms = (GET_MILLIS() - offset_ms);
  uint32_t since_last_random = now_ms - modifier->last_random_ms;
  uint32_t period_ms = 256 * 256 * 256 / modifier->beats_per_minute;

  if (modifier->type == MODIFIER_RANDOM_TRANSITIONS)
  {
    float progress = float(since_last_random) / float(period_ms);
    uint16_t new_value = easeOutQuart(modifier->last_random_value, modifier->next_random_value, progress);
    set_value(modifier, new_value, command_buffer);
  }

  // change random value every x milliseconds
  if (since_last_random > period_ms)
  {
    uint16_t new_value = random16(modifier->min * 256, modifier->max * 256);
    modifier->last_random_ms = now_ms;
    if (modifier->type == MODIFIER_RANDOM)
    {
      set_value(modifier, new_value, command_buffer);
    }
    else if (modifier->type == MODIFIER_RANDOM_TRANSITIONS)
    {
      modifier->last_random_value = modifier->next_random_value;
      modifier->next_random_value = new_value;
    }
  }
}
