#include <Dode.h>
#include <Commands.h>

#define MAX_BALLS 16

struct Ball
{
  int8_t last_edge;
  int8_t current_edge;
  int8_t next_edge;

  float position;
};

Ball balls[MAX_BALLS];
uint8_t active_balls = 0;
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

void add_ball()
{
  if (active_balls < MAX_BALLS)
  {
    uint8_t index = active_balls;
    balls[index].last_edge = 1;
    balls[index].current_edge = 1;
    balls[index].next_edge = 1;
    balls[index].position = random8(0, EDGE_PIXEL_COUNT);
    active_balls++;
  }
}

void render_ball(Leds *leds, int8_t edge, float center, float width, CRGB color, float brightness)
{
  if (edge < 0)
  {
    edge = edge * -1;
    center = EDGE_PIXEL_COUNT - center;
  }
  Commands::render_ball(leds, edge - 1, center, width, color, brightness);
}

void Dode::random_walk(char *data)
{
  uint8_t color_index = data[0];
  float brightness = data[1];
  uint8_t rate = data[2];
  float width = float(data[3]) / 10.0;
  uint8_t ball_count = data[4];

  if (active_balls > ball_count)
  {
    active_balls = ball_count;
  }
  else if (active_balls < ball_count)
  {
    for (int i = 0; i < ball_count - active_balls; i++)
    {
      add_ball();
    }
  }

  long now = millis();

  for (int i = 0; i < active_balls; i++)
  {
    CRGB color = ColorFromPalette(currentPalette, color_index + i * 256 / active_balls, 255);
    balls[i].position = balls[i].position + float(rate) * float(now - last_update) / 500.0;

    if (balls[i].position > EDGE_PIXEL_COUNT)
    {
      balls[i].last_edge = balls[i].current_edge;
      balls[i].current_edge = balls[i].next_edge;
      balls[i].next_edge = next_edge(balls[i].current_edge);
      balls[i].position = balls[i].position - EDGE_PIXEL_COUNT;
    }

    render_ball(this, balls[i].last_edge, balls[i].position + EDGE_PIXEL_COUNT, width, color, brightness / 255.0);
    render_ball(this, balls[i].current_edge, balls[i].position, width, color, brightness / 255.0);
    render_ball(this, balls[i].next_edge, balls[i].position - EDGE_PIXEL_COUNT, width, color, brightness / 255.0);
  }
  last_update = now;
}
