#include <Dode.h>
#include <Commands.h>

void Dode::kaleidoscope(char *data)
{

  for (int i = 0; i < EDGE_COUNT; i++)
  {
    uint8_t length = strip_length(i);
    for (int j = 0; j < length / 2; j++)
    {
      set_led(i, length - j, get_led(i, j));
    }
  }
}
