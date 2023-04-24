#ifndef MODIFIER_H /* include guards */
#define MODIFIER_H

#include <command_schemas.pb.h>

#define MAX_MODIFIERS 10

class Modifiers
{

public:
  static void test();
  static void apply_all(CommandParams *cmd);
  static void apply(Modifier modifier, int *value, uint8_t command_index);
  static void apply_envelope(Modifier modifier, int *value, uint8_t command_index);
  static void apply_random(Modifier modifier, int *value, uint8_t command_index);
  static void apply_beat(Modifier modifier, int *value, uint8_t band);

  // generated
  static int *value_from_type_param(CommandParams *cmd, uint8_t field_index);
};

#endif /* MODIFIER_H */
