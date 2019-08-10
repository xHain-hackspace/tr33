#include <Commands.h>
#include <Tr33.h>

void Tr33::ping_pong(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t ball_type = data[1];
  uint8_t color_index = data[2];
  uint8_t brightness = data[3];
  float width = float(data[4]) / 10.0;
  uint8_t period_100ms = data[5];
  uint8_t offset_100ms = data[6];

  float center = strip_length(strip_index) * Commands::ping_pong_linear(period_100ms, offset_100ms);
  CRGB color = ColorFromPalette(currentPalette, color_index, brightness);

  render_ball(strip_index, ball_type, center, width, color, 1, true, true);
}