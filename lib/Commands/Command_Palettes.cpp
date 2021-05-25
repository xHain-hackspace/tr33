#include <command_schemas.pb.h>
#include <Command_Palettes.h>

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color)
{
  return color_from_palette(cmd, color, 255);
}

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color, uint8_t brightness)
{
  CRGBPalette256 palette = Rainbow_gp;

  switch (cmd.color_palette)
  {
  case ColorPalette_RAINBOW:
    palette = Rainbow_gp;
    break;
  case ColorPalette_FOREST:
    palette = ForestColors_p;
    break;
  case ColorPalette_OCEAN:
    palette = OceanColors_p;
    break;
  case ColorPalette_PARTY:
    palette = PartyColors_p;
    break;
  case ColorPalette_HEAT:
    palette = HeatColors_p;
    break;
  case ColorPalette_SPRING_ANGEL:
    palette = springangel_gp;
    break;
  case ColorPalette_SCOUTY:
    palette = scoutie_gp;
    break;
  case ColorPalette_PURPLE_HEAT:
    palette = purple_heat;
    break;
  case ColorPalette_LAVA:
    palette = LavaColors_p;
    break;
  case ColorPalette_CLOUD:
    palette = CloudColors_p;
    break;
  }

  return ColorFromPalette(palette, color, brightness);
}
