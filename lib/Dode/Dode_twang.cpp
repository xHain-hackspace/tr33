#include <Dode.h>
#include <Twang.h>

void Dode::twang(uint8_t *data)
{
    twang_loop();
    twang_render_dode(this);
}

void Dode::joystick(uint8_t *data)
{
    twang_joystick(data);
}
