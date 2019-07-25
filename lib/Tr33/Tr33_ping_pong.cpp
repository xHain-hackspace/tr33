#include <Commands.h>
#include <Tr33.h>

void Tr33::ping_pong(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t ball_type = data[1];
  uint8_t color_index = data[2];
  uint8_t brightness = data[3];
  float width = float(data[4]) / 10.0;
  uint8_t bpm = data[5];
  float rel_offset = float(data[6]) / 100.0;

  float length = strip_index_length(strip_index);
  float offset = rel_offset * length * 2;
  float rate = length / (60.0 / bpm);
  float total_distance = offset + float(millis()) / 1000.0 * rate;
  float center = fmod(total_distance, (length * 2.0));

  CRGB color = ColorFromPalette(currentPalette, color_index, brightness);

  if (center > length)
  {
    center = 2.0 * (length - 1.0) - center;
    width = -1.0 * width;
  }

  render_ball(strip_index, ball_type, center, width, color, 1, true, true);
}