#include <Commands.h>
#include <Modifiers.h>

void Commands::beat_equalizer(LedStructure *leds, CommandParams cmd)
{
  BeatEqualizer beat_equalizer = cmd.type_params.beat_equalizer;

  for (int i = 0; i < leds->strip_length(cmd.strip_index); i++)
  {
    leds->fade_led(cmd, i, color_from_palette(cmd, beat_equalizer.color));
  }
}