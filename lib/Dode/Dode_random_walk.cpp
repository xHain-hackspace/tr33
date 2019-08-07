#include <Dode.h>
#include <Commands.h>

struct Ball
{
  int8_t last_edge;
  int8_t current_edge;
  int8_t next_edge;

  float position;
};

Ball ball = {1, 1, 1, 0};
long last_update = millis();

int8_t next_edge(int8_t current_edge)
{
  uint8_t rand_number = random8(0, 2);
  uint8_t current_edge_index = abs8(current_edge) - 1;

  if (current_edge > 0 && rand_number == 0)
  {
    return corners[current_edge_index].top_left;
  }
  else if (current_edge > 0 && rand_number == 1)
  {
    return corners[current_edge_index].top_right;
  }
  else if (current_edge < 0 && rand_number == 0)
  {
    return corners[current_edge_index].bottom_left;
  }
  else if (current_edge < 0 && rand_number == 1)
  {
    return corners[current_edge_index].bottom_right;
  }
};

void Dode::random_walk(char *data)
{
  uint8_t color_index = data[0];
  uint8_t rate = data[1];
  float width = float(data[2]) / 10.0;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);

  long now = millis();
  ball.position = ball.position + float(rate) * float(now - last_update) / 500.0;

  if (ball.position > EDGE_PIXEL_COUNT)
  {
    ball.last_edge = ball.current_edge;
    ball.current_edge = ball.next_edge;
    ball.position = ball.position - EDGE_PIXEL_COUNT;

    ball.next_edge = next_edge(ball.current_edge);
  }

  render_sine_ball(ball.last_edge, ball.position + EDGE_PIXEL_COUNT, width, color, 1.0);
  render_sine_ball(ball.current_edge, ball.position, width, color, 1.0);
  render_sine_ball(ball.next_edge, ball.position - EDGE_PIXEL_COUNT, width, color, 1.0);
  last_update = now;
}
