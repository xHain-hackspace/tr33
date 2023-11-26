#include <Commands.h>

void Commands::kaleidoscope(LedStructure *leds, const CommandParams &cmd)
{
  uint16_t length = leds->strip_length(cmd.strip_index);

  for (int i = 0; i < length / 2; i++)
  {
    leds->fade_led(cmd, length - i - 1, leds->get_led(cmd.strip_index, i));
  }
}
