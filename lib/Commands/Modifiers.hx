#ifndef MODIFIER_H /* include guards */
#define MODIFIER_H

#include <Arduino.h>
#include <Commands.h>

class Modifiers
{

public:
  static void test();
  static void apply(CommandParams cmd);
  static void apply_envelope(CommandParams cmd);
  static void apply_random(CommandParams cmd);
};

#endif /* MODIFIER_H */
