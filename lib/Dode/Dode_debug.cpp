#include <Dode.h>
#include <Commands.h>

void edge(Leds *leds, int8_t index, CRGB color)
{
  if (index > 0)
  {
    for (int i = 0; i < 30; i++)
    {
      leds->set_led(index - 1, i, color);
    }
  }
  else
  {
    for (int i = 40; i < 70; i++)
    {
      leds->set_led(abs(index) - 1, i, color);
    }
  }
}

void Dode::debug(char *data)
{
  uint8_t edge_index = data[0];

  for (int i = 0; i < 30; i++)
  {
    set_led(edge_index, i, CHSV(HUE_GREEN, 255, 255));
  }

  for (int i = 40; i < 70; i++)
  {
    set_led(edge_index, i, CHSV(HUE_RED, 255, 255));
  }

  edge(this, corners[edge_index].bottom_left, CHSV(HUE_AQUA, 255, 255));
  edge(this, corners[edge_index].bottom_right, CHSV(HUE_BLUE, 255, 255));

  edge(this, corners[edge_index].top_left, CHSV(HUE_ORANGE, 255, 255));
  edge(this, corners[edge_index].top_right, CHSV(HUE_PINK, 255, 255));
}
