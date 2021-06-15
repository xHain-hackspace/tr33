#include <Commands.h>

#define GRAVITY_MAX_BALLS 50
#define GRAVITY_VALUE 50
#define GRAVITY_DAMPING 70

int gravity_last_ball[COMMAND_COUNT];

struct Ball
{
  bool enabled;
  int start;
  int last_update;
  float position;
  float rate;
  float width;
  bool bounce = true;

  CRGB color;
  uint8_t strip_index;
  uint8_t command_index;
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

void new_ball(LedStructure *leds, CommandParams cmd)
{
  Gravity gravity = cmd.type_params.gravity;

  Ball ball;
  ball.enabled = true;
  ball.last_update = millis();
  ball.start = millis();
  ball.position = 0;

  ball.strip_index = cmd.strip_index;
  ball.rate = Commands::random_or_value(gravity.launch_speed, 30, 120);
  ball.color = Commands::color_from_palette(cmd, Commands::random_or_value(gravity.color, 0, 255));

  gravity_last_ball[cmd.index] = millis();

  balls[next_ball] = ball;
  next_ball++;
  if (next_ball >= GRAVITY_MAX_BALLS)
  {
    next_ball = 0;
  }
}

void Commands::gravity(LedStructure *leds, CommandParams cmd)
{
  Gravity gravity = cmd.type_params.gravity;
  uint8_t frequency = gravity.ball_rate;

  if (frequency > 0 && gravity_last_ball[cmd.index] < millis() && 10000 / (millis() - gravity_last_ball[cmd.index]) < frequency)
  {
    new_ball(leds, cmd);
  }

  for (int i = 0; i < GRAVITY_MAX_BALLS; i++)
  {
    if (balls[i].enabled && balls[i].command_index == cmd.index)
    {
      update_ball(i);
      float width = balls[i].rate * float(gravity.width) / 1500.0;
      render_comet(leds, balls[i].strip_index, balls[i].position, width, balls[i].color, 1.0, true, false, false);
    }
  }
}