#include <Commands.h>

#define GRAVITY_VALUE 50.0
#define GRAVITY_DAMPING 0.7
#define MAX_BALL_DURATION_MS 40000

uint32_t gravity_last_ball_ms[COMMAND_COUNT];

void Commands::gravity(LedStructure *leds, CommandParams cmd)
{
  Gravity gravity = cmd.type_params.gravity;
  float frequency = float(gravity.ball_rate) / 1000.0;
  uint32_t now = millis();

  // new ball
  if (frequency > 0 && (250.0 / float(max((uint32_t)1, now - gravity_last_ball_ms[cmd.index]))) < frequency)
  {
    effect_item_index++;
    if (effect_item_index >= EFFECT_ITEM_COUNT)
    {
      effect_item_index = 0;
    }

    EffectItem *new_ball = &effect_items[effect_item_index];

    CRGB new_color;
    switch (gravity.color_type)
    {
    case ColorType_SINGLE_COLOR:
      new_color = color_from_palette(cmd, gravity.color);
      break;
    case ColorType_RANDOM_COLOR:
      new_color = color_from_palette(cmd, random(0, 255));
      break;
    case ColorType_WHITE:
      new_color = COLOR_WHITE;
      break;
    }

    new_ball->enabled = true;
    new_ball->color = new_color;
    new_ball->strip_index = cmd.strip_index;
    new_ball->command_index = cmd.index;
    new_ball->param_1 = (float(gravity.launch_speed) * (1.0 + float(random(0, 200) / 1000.0)) / 1.2) * float(leds->strip_length(cmd.strip_index)) / 300.0; // rate
    new_ball->center = 0;
    new_ball->start_time = now;
    new_ball->type = CommandParams_gravity_tag;

    gravity_last_ball_ms[cmd.index] = now;
  }

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    EffectItem *ball = &effect_items[i];

    if (ball->enabled && ball->command_index == cmd.index && ball->type == CommandParams_gravity_tag)
    {
      // update
      float time_diff = float(now - ball->start_time) / 1000.0;
      float center = float(ball->center) + float(ball->param_1) * time_diff - 0.5 * GRAVITY_VALUE * time_diff * time_diff;
      float rate = float(ball->param_1) - GRAVITY_VALUE * time_diff;
      float width = rate * float(gravity.width) / 500.0;

      if (center < 0)
      {
        ball->enabled = fabs(rate) > 5;
        ball->center = fabs(center);
        ball->param_1 = fabs(rate) * GRAVITY_DAMPING;
        ball->start_time = now;
      }

      // render
      render_comet(leds, ball->strip_index, center, width, ball->color, 1.0, true, false, false, false);
    }
  }
}