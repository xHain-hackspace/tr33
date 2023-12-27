#include <Commands.h>
#include <Modifiers.h>

#define WALK_DIR_UP 0
#define WALK_DIR_DOWN 1

int8_t get_next_edge(int8_t current_edge)
{
  // find corner
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      if (corners[i][j] == current_edge)
      {
        // find random edge
        uint8_t rand_number = random8(0, 2);
        if (rand_number == 0)
        {
          return corners[i][j + 1];
        }
        else
        {
          return corners[i][j + 2];
        }
      }
    }
    if (corners[i][0] == current_edge)
    {
      // find random edge
      uint8_t rand_number = random8(0, 2);
      if (rand_number == 0)
      {
        return corners[i][1];
      }
      else
      {
        return corners[i][2];
      }
    }
  }
};

void Commands::random_walk(LedStructure *leds, const CommandParams &cmd)
{
  RandomWalk random_walk = cmd.type_params.random_walk;

  float brightness = float(cmd.brightness) / 255;
  float rate = float(random_walk.speed) / 10.0;

  uint8_t min_led = 0;
  uint8_t max_led = leds->strip_length(cmd.strip_index) - 1;

  uint32_t now = millis();

  CRGB color = color_from_palette(cmd, random_walk.color);

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    EffectItem *ball = &effect_items[i];

    if (ball->enabled && ball->command_index == cmd.index && ball->type == CommandParams_random_walk_tag)
    {
      uint32_t time_diff = (now - ball->start_time);
      float distance = float(time_diff) / 1000.0 * float(rate);

      if (ball->param_1 == WALK_DIR_DOWN)
      {
        distance = -distance;
      }
      float center = ball->center - distance;

      // generate new ball if out of bounds
      if (ball->param_2 == 0 && (center < min_led || center > max_led))
      {

        ball->param_2 = 1;
      }

      // disable ball if out of bounds
      if (center > leds->strip_length(cmd.strip_index) + 60 || center < -60)
      {
        ball->enabled = false;
      }
      else
      {
        render(leds, random_walk.shape, ball->strip_index, center, random_walk.width, random_walk.color, brightness, ball->param_1 == WALK_DIR_DOWN, false, false);
      }
    }
  }
}