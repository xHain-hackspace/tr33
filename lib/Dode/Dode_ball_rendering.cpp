#include <Dode.h>
#include <Commands.h>

uint16_t Dode::edge_strip_length(Leds *leds, int8_t edge)
{
  return leds->strip_length(abs8(edge) - 1);
}

void Dode::render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness, uint8_t ball_type)
{
  bool reverse = false;

  if (edge < 0)
  {
    edge = edge * -1;
    center = edge_strip_length(leds, edge) - center;
    reverse = true;
  }

  switch (ball_type)
  {
  case 0:
  case BALL_TYPE_SINE:
    Commands::render_ball(leds, edge - 1, center, width, color, brightness);
    break;
  case BALL_TYPE_COMET:
    Commands::render_comet(leds, edge - 1, center, width, color, brightness, reverse);
    break;
  case BALL_TYPE_NYAN:
    Commands::render_nyan(leds, edge - 1, center, width, color, brightness, reverse);
    break;
  }
}