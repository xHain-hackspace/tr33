#include <Dode.h>
#include <Twang.h>

void Dode::twang(char *data)
{
    twang_loop();
    twang_render_dode(this);
}

void Dode::joystick(char *data)
{
    twang_joystick(data);
}
