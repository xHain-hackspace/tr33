#include <Commands.h>
#include <Twang.h>

void Commands::twang(LedStructure *leds)
{
  twang_loop();
  twang_render(leds);
}

void Commands::twang_joystick(uint8_t *data)
{
  twang_joystick_update((int8_t)data[0], (uint8_t)data[1]);
}
