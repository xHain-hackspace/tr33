#include <Commands.h>

// Gradient palette "scoutie_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/scoutie.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
DEFINE_GRADIENT_PALETTE( scoutie_gp ) {
    0, 255,156,  0,
  127,   0,195, 18,
  216,   1,  0, 39,
  255,   1,  0, 39};

// Gradient palette "springangel_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/springangel.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.
DEFINE_GRADIENT_PALETTE( springangel_gp ) {
    0,  95,151,140,
   48,  35,101,170,
   71,  74,103,111,
   94,  77, 97,111,
  117, 165,142,132,
  140, 186,142,128,
  163, 182,124, 96,
  186, 188, 95, 57,
  209, 220, 88, 28,
  232,  92, 86, 41,
  255,  92, 86, 41};

void Commands::update_settings(char * data) {
  switch (data[0]) {
    case 0: currentPalette = Rainbow_gp; break; 
    case 1: currentPalette = ForestColors_p; break; 
    case 2: currentPalette = OceanColors_p; break; 
    case 3: currentPalette = PartyColors_p; break; 
    case 4: currentPalette = HeatColors_p; break; 
    case 5: currentPalette = springangel_gp; break;
    case 6: currentPalette = scoutie_gp; break;
  }

  switch (data[1]) {
    case 0: FastLED.setTemperature(UncorrectedTemperature); break;
    case 1: FastLED.setTemperature(Candle); break;  /* 1900 K, 255, 147, 41 */
    case 2: FastLED.setTemperature(Tungsten40W); break; /* 2600 K, 255, 197, 143 */
    case 3: FastLED.setTemperature(Tungsten100W); break; /* 2850 K, 255, 214, 170 */
    case 4: FastLED.setTemperature(Halogen); break; /* 3200 K, 255, 241, 224 */
    case 5: FastLED.setTemperature(CarbonArc); break; /* 5200 K, 255, 250, 244 */
    case 6: FastLED.setTemperature(HighNoonSun); break; /* 5400 K, 255, 255, 251 */
    case 7: FastLED.setTemperature(DirectSunlight); break; /* 6000 K, 255, 255, 255 */
    case 8: FastLED.setTemperature(OvercastSky); break; /* 7000 K, 201, 226, 255 */
    case 9: FastLED.setTemperature(ClearBlueSky); break; /* 20000 K, 64, 156, 255 */
  }
};
