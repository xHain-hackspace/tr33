#include <command_schemas.pb.h>
#include <Command_Palettes.h>
#include <Command_Palettes_Wled.h>

CRGBPalette32 palettes[_ColorPalette_MAX];

void Commands::load_palettes()
{
  for (int i = 0; i < _ColorPalette_MAX; i++)
  // for (int i = 0; i < ColorPalette_PARROT; i++)
  {
    switch (i)
    {
      // fastled
    case ColorPalette_RAINBOW:
      palettes[i] = RainbowColors_p;
      break;
    case ColorPalette_FOREST:
      palettes[i] = ForestColors_p;
      break;
    case ColorPalette_OCEAN:
      palettes[i] = OceanColors_p;
      break;
    case ColorPalette_PARTY:
      palettes[i] = PartyColors_p;
      break;
    case ColorPalette_HEAT:
      palettes[i] = HeatColors_p;
      break;
    case ColorPalette_LAVA:
      palettes[i] = LavaColors_p;
      break;
    case ColorPalette_CLOUD:
      palettes[i] = CloudColors_p;
      break;
      // custom
    case ColorPalette_SCOUTY:
      palettes[i] = scoutie_gp;
      break;
    case ColorPalette_PURPLE_HEAT:
      palettes[i] = purple_heat;
      break;
    case ColorPalette_SPRING_ANGEL:
      palettes[i] = springangel_gp;
      break;
    case ColorPalette_SAGA:
      palettes[i] = saga_02_gp;
      break;
    case ColorPalette_SAGA2:
      palettes[i] = saga_14_gp;
      break;
    case ColorPalette_PARROT:
      palettes[i] = parrot_gp;
      break;
      // wled
    default:
      palettes[i] = wled_palettes[i - (ColorPalette_PARROT + 1)];
    }
  }
}

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color_index)
{
  return color_from_palette(cmd, color_index, 255);
}

CRGB Commands::color_from_palette(CommandParams cmd, uint8_t color_index, uint8_t brightness)
{
  return ColorFromPalette(palettes[cmd.color_palette], color_index, brightness);
}
