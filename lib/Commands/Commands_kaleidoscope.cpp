#include <Commands.h>

void Commands::kaleidoscope(LedStructure *leds, CommandParams cmd)
{
  uint16_t length = leds->strip_length(cmd.strip_index);

  for (int i = 0; i < length / 2; i++)
  {
    leds->set_led(cmd.strip_index, length - i - 1, leds->get_led(cmd.strip_index, i));
  }
}
