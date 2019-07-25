#include <Tr33.h>
#include <Commands.h>

// TODO: This is not finished at all

float height = 50;
int last_update;

void Tr33::beats(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t color_index = data[1];
  uint8_t rate = data[2];

  // for (int i=0; i<height; i++) {
  //   set_led(strip_index, i, ColorFromPalette(currentPalette, color_index));
  // }

  render_ball(strip_index, BALL_TYPE_COMET, height, 1, ColorFromPalette(currentPalette, color_index), 1, false, false);

  int now = millis();
  int interval = now - last_update;
  height = height - float(rate) * float(interval) / 1000.0;

  if (height <= 0)
  {
    beat(0);
  }

  last_update = now;
}

void Tr33::beat(char *data)
{
  // height = data[0];
  // height = random8(0, 100);
  height = 40;
}