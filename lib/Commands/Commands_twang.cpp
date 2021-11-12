#include <Commands.h>
#include <Twang.h>

void Commands::twang(LedStructure *leds)
{
  int movement = int(float(joystick_y) / 255.0 * 180.0) - 90;
  twang_loop(movement, joystick_button);
  twang_render(leds);
}