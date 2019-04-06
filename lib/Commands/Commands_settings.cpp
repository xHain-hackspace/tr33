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

// Gradient palette "bhw1_05_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_05.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE( purple_heat ) {
    0,   4,  2,  9,
   58,  16,  0, 47,
  122,  24,  0, 16,
  158, 144,  9,  1,
  183, 179, 45,  1,
  219, 220,114,  2,
  255, 234,237,  1};

  // Gradient palette "parrot_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/rc/tn/parrot.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( parrot_gp ) {
    0, 126,  0,  1,
  114, 197,168, 16,
  140, 197,168, 16,
  216,   0,  2, 32,
  255,   0,  2, 32};

// Gradient palette "saga_14_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/saga/tn/saga-14.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 80 bytes of program space.

DEFINE_GRADIENT_PALETTE( saga_14_gp ) {
    0,   0,  0,255,
   13,   1,  0,221,
   26,   1,  0,192,
   40,   2,  0,164,
   53,   4,  0,140,
   67,   7,  0,117,
   80,  12,  0, 98,
   93,  18,  0, 80,
  107,  26,  0, 64,
  120,  35,  0, 51,
  134,  47,  0, 38,
  147,  60,  0, 29,
  161,  77,  0, 20,
  174,  94,  0, 14,
  187, 113,  0,  9,
  201, 137,  0,  5,
  214, 161,  0,  2,
  228, 190,  0,  1,
  241, 220,  0,  1,
  255, 255,  0,  0};

// Gradient palette "saga_02_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/saga/tn/saga-02.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 80 bytes of program space.

DEFINE_GRADIENT_PALETTE( saga_02_gp ) {
    0,   6, 11,255,
   13,   4, 14,255,
   26,   2, 21,255,
   40,   1, 31,255,
   53,   1, 41,255,
   67,   0, 54,250,
   80,   1, 79,182,
   93,   1,101,123,
  107,   2,118, 77,
  120,   7,128, 43,
  134,  19,133, 21,
  147,  40,128,  8,
  161,  73,118,  2,
  174, 120,101,  1,
  187, 179, 79,  1,
  201, 249, 55,  0,
  214, 255, 41,  1,
  228, 255, 31,  1,
  241, 255, 21,  2,
  255, 255, 14,  5};


void Commands::update_settings(char * data) {
  switch (data[0]) {
    case 0: currentPalette = Rainbow_gp; break; 
    case 1: currentPalette = ForestColors_p; break; 
    case 2: currentPalette = OceanColors_p; break; 
    case 3: currentPalette = PartyColors_p; break; 
    case 4: currentPalette = HeatColors_p; break; 
    case 5: currentPalette = springangel_gp; break;
    case 6: currentPalette = scoutie_gp; break;
    case 7: currentPalette = purple_heat; break;
    case 8: currentPalette = parrot_gp; break;
    case 9: currentPalette = saga_14_gp; break;
    case 10: currentPalette = saga_02_gp; break;
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
