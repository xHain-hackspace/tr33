#include <Commands.h>

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70

uint8_t gravity_color_index = 0;
uint8_t gravity_width = 0;
uint8_t gravity_rate = 0;
uint8_t gravity_strip_index = 0;
int gravity_last_ball = 0;

struct Ball
{
  bool enabled;
  int start;
  int last_update;
  float position;
  float rate;
  float width;
  bool bounce = true;

  uint8_t color_index;
  uint8_t strip_index;
};

Ball balls[GRAVITY_MAX_BALLS];
int next_ball = 0;

void update_ball(int i)
{
  int now = millis();
  float interval = float(now - balls[i].last_update) / 1000.0;

  balls[i].last_update = now;
  balls[i].position = float(balls[i].position) + float(balls[i].rate) * interval + 0.5 * float(GRAVITY_VALUE) * interval * interval;
  balls[i].rate = balls[i].rate - float(GRAVITY_VALUE) * interval;

  if (balls[i].position < 0)
  {
    if (balls[i].bounce)
    {
      balls[i].enabled = fabs(balls[i].rate) > 20 && now - balls[i].start < 40000;
      balls[i].position = fabs(balls[i].position);
      balls[i].rate = fabs(balls[i].rate) * (1.0 - GRAVITY_DAMPING / 255.0);
    }
    else
    {
      balls[i].enabled = false;
    }
  }
}

void Commands::gravity_event(LedStructure *leds, uint8_t *data)
{
  Ball ball;
  ball.enabled = true;
  ball.last_update = millis();
  ball.start = millis();
  ball.position = 0;

  ball.strip_index = gravity_strip_index;
  ball.rate = Commands::random_or_value(gravity_rate, 30, 120);
  ball.color_index = Commands::random_or_value(gravity_color_index, 0, 255);

  gravity_last_ball = millis();

  balls[next_ball] = ball;
  next_ball++;
  if (next_ball >= GRAVITY_MAX_BALLS)
  {
    next_ball = 0;
  }
}

void Commands::gravity(LedStructure *leds, uint8_t *data)
{
  gravity_strip_index = data[0];
  gravity_color_index = data[1];
  gravity_rate = data[2];
  int frequency = data[3];
  gravity_width = data[4];

  if (frequency > 0 && gravity_last_ball < millis() && 10000 / (millis() - gravity_last_ball) < frequency)
  {
    gravity_event(leds, data);
  }

  for (int i = 0; i < GRAVITY_MAX_BALLS; i++)
  {
    if (balls[i].enabled)
    {
      update_ball(i);
      CRGB color = ColorFromPalette(currentPalette, balls[i].color_index);
      float width = balls[i].rate * float(gravity_width) / 1500.0;
      render_comet(leds, balls[i].strip_index, balls[i].position, width, color, 1.0, true, false, false);
    }
  }
}