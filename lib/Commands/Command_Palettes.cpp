#include <command_schemas.pb.h>
#include <Command_Palettes.h>
#include <Command_Palettes_Wled.h>

#define PALETTE_COUNT 11
CRGBPalette32 palettes[PALETTE_COUNT];

uint8_t tag_to_index(uint8_t tag)
{
  switch (tag)
  {
  case ColorPalette_RAINBOW:
    return 0;
  case ColorPalette_FOREST:
    return 1;
  case ColorPalette_OCEAN:
    return 2;
  case ColorPalette_PARTY:
    return 3;
  case ColorPalette_SCOUTY:
    return 4;
  case ColorPalette_RIVENDELL:
    return 5;
  case ColorPalette_ORANGE_TEAL:
    return 6;
  case ColorPalette_ORANGERY:
    return 7;
  case ColorPalette_SAKURA:
    return 8;
  case ColorPalette_AURORA:
    return 9;
  case ColorPalette_ATLANTICA:
    return 10;
  default:
    return 0;
  }
}

void Commands::load_palettes()
{
  palettes[0] = RainbowColors_p;
  palettes[1] = ForestColors_p;
  palettes[2] = OceanColors_p;
  palettes[3] = PartyColors_p;
  palettes[4] = scoutie_gp;
  palettes[5] = wled_palettes[0];
  palettes[6] = wled_palettes[1];
  palettes[7] = wled_palettes[2];
  palettes[8] = wled_palettes[3];
  palettes[9] = wled_palettes[4];
  palettes[10] = wled_palettes[5];
}

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color_index)
{
  return color_from_palette(cmd, color_index, 255);
}

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color_index, uint8_t brightness)
{
  return color_from_palette(cmd.color_palette, color_index, brightness);
}

CRGB Commands::color_from_palette(ColorPalette palette_tag, uint8_t color_index, uint8_t brightness)
{
  return ColorFromPalette(palettes[tag_to_index(palette_tag)], color_index, brightness);
}