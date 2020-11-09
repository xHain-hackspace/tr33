#ifndef MODIFIER_H /* include guards */
#define MODIFIER_H

#include <Arduino.h>
#include <Commands.h>

#define MODIFIER_COUNT 16

// -- TYPES ------------------------------------------------------------------------------

#define MODIFIER_DISABLED 0
#define MODIFIER_LINEAR 1
#define MODIFIER_SINE 2
#define MODIFIER_QUADRATIC 3
#define MODIFIER_CUBIC 4
#define MODIFIER_SAWTOOTH 5
#define MODIFIER_SAWTOOTH_REVERSE 6
#define MODIFIER_RANDOM 7
#define MODIFIER_RANDOM_TRANSITIONS 8
#define MODIFIER_BOUNCE 9

struct Modifier
{
  // cast from command
  uint8_t type;
  uint8_t command_index;
  uint8_t data_index;
  uint8_t data_bytes;
  accum88 beats_per_minute;
  uint16_t offset_100ms;
  uint8_t max;
  uint8_t min;

  // internal
  uint32_t last_random_ms;
  uint16_t last_random_value;
  uint16_t next_random_value;

  // alternative to accum88? ms = (x/80)^2+50
};

class Modifiers
{
private:
  static Modifier modifiers[MODIFIER_COUNT];

public:
  static void test();
  static void apply(Command command_buffer[]);
  static void apply_envelope(Modifier *modifier, Command command_buffer[]);
  static void apply_random(Modifier *modifier, Command command_buffer[]);
};

#endif /* MODIFIER_H */
