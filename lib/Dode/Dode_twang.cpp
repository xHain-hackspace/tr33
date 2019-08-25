#include <Dode.h>
#include <Twang.h>

void Dode::twang(char *data)
{
    twang_loop();
    twang_render(this);
}

void Dode::twang_joystick(char *data) 
{
    twang_set_tilt(data[0]);
    twang_set_wobble(data[1]);
}
