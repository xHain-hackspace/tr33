#include <Arduino.h>
#include <Beats.h>
#include <MD_MSGEQ7.h>

MD_MSGEQ7 MSGEQ7(MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_DATA_PIN);

static long msgeq7_then = 0;
uint16_t msgeq7_bands[MAX_BAND + 1]; // +1 for 'overall' band (calculated)
bool msgeq7_initialized = false;

uint16_t Beats::read_band(uint8_t index)
{
  long msgeq7_now = millis();

  //intialize if not already initialized
  if (!msgeq7_initialized)
  {
    MSGEQ7.begin();
    analogReadResolution(10);
  }

  // read equalizer values into array, also calculate overall value of all bands
  if (msgeq7_now > msgeq7_then + MSGEQ7_READ_DELAY)
  {
    uint16_t overall_band = 0;
    MSGEQ7.read();

    for (uint8_t i = 0; i < MAX_BAND; i++)
    {
      msgeq7_bands[i] = MSGEQ7.get(i);
      overall_band += msgeq7_bands[i]/MAX_BAND;
    }
    msgeq7_bands[MAX_BAND] = overall_band; //set 'extra' band to overall value
    msgeq7_then = msgeq7_now;
  }

  if (index < MAX_BAND+1)
  {
    return map(msgeq7_bands[index], 1, 1023, 0, 256 * 256 - 1);
  }
  else
  {
    return 0;
  }
}
