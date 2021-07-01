#include <MD_MSGEQ7.h>     
#include <Commands.h>
#include <Modifiers.h>

// hardware pin definitions - change to suit circuit
#define MSGEQ7_DATA_PIN    34
#define MSGEQ7_RESET_PIN   27
#define MSGEQ7_STROBE_PIN  26

// frequency reading the IC data
#define MSGEQ7_READ_DELAY  15

MD_MSGEQ7 MSGEQ7(MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_DATA_PIN);

static long msgeq7_then = 0;
uint16_t msgeq7_bands[MAX_BAND];
bool msgeq7_initialized = false;

void Commands::single_color(LedStructure *leds, CommandParams cmd)
{
  //SingleColor single_color = cmd.type_params.single_color;
  long msgeq7_now = millis();

  //intialize if not already initialized
  if (!msgeq7_initialized){
    MSGEQ7.begin();
    analogReadResolution(10);
  }

  // read equalizer values into array
  if(msgeq7_now > msgeq7_then + MSGEQ7_READ_DELAY) {

    MSGEQ7.read();

    //Serial.println("---");
    for (uint8_t i=0; i<MAX_BAND; i++)
    {
      msgeq7_bands[i] = MSGEQ7.get(i);
      //Serial.println(msgeq7_bands[i]);
    }
    //Serial.println("---");

    msgeq7_then = msgeq7_now;
  }

  //calculate color value from selected band
  uint16_t value = msgeq7_bands[1];
  uint8_t scaled_value = value < 130 ? 0 : map(value, 100, 1023, 0, 255);
  for (int i = 0; i < leds->strip_length(cmd.strip_index); i++)
  {
    leds->fade_led(cmd, i, color_from_palette(cmd, scaled_value));
  }
}