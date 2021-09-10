
#include <Commands.h>
#include <Beats.h>

void Commands::beat_equalizer(LedStructure *leds, CommandParams cmd)
{
  BeatEqualizer beat_equalizer = cmd.type_params.beat_equalizer;

  uint16_t value = Beats::read_band(beat_equalizer.band);
  uint8_t scaled_value = value < 130 ? 0 : map(value, 100, 1023, 0, 255);

  for (int i = 0; i < leds->strip_length(cmd.strip_index) * scaled_value / 255; i++)
  {
    leds->fade_led(cmd, i, color_from_palette(cmd, beat_equalizer.color));
  }
}