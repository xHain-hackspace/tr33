#include <Keller.h>
#include <Commands.h>


void Keller::render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness, uint8_t ball_type)
{
  bool reverse = false;

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