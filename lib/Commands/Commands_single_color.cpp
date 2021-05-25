#include <Commands.h>

void Commands::single_color(LedStructure *leds, CommandParams cmd)
{
  for (int i = 0; i < leds->strip_length(cmd.strip_index); i++)
  {
    leds->fade_led(cmd, i, color_from_palette(cmd, cmd.type_params.single_color.color));
  }
}