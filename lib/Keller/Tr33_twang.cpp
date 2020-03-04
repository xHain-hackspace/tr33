#include <Tr33.h>
#include <Twang.h>

void Tr33::twang(char *data)
{
    twang_loop();
    twang_render_tr33(this);
}

void Tr33::joystick(char *data)
{
    twang_joystick(data);
}
