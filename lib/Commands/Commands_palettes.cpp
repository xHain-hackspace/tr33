#include <Commands.h>

void Commands::set_palette(char * data) {
  switch (data[0]) {
    case 0: currentPalette = Rainbow_gp; break; 
    case 1: currentPalette = CloudColors_p; break; 
    case 2: currentPalette = ForestColors_p; break; 
    case 3: currentPalette = LavaColors_p; break; 
    case 4: currentPalette = OceanColors_p; break; 
    case 5: currentPalette = PartyColors_p; break; 
    case 6: currentPalette = HeatColors_p; break; 
  }
};