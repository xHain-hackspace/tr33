#include <Commands.h>
#include <Modifiers.h>
#include <Tetraeder.h>
#include <Network.h>

#define WALK_DIR_UP 0
#define WALK_DIR_DOWN 1

int8_t find_next_edge(int8_t current_edge)
{
  // find corner
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      if (corners[i][j] == current_edge)
      {
        // find next edge
        uint8_t edge_count = 0;
        for (int k = 0; k < 6; k++)
        {
          if (corners[i][k] != NO_EDGE)
          {
            edge_count++;
          }
        }

        return corners[i][random8(0, edge_count)];
      }
    }
  }
};

void Commands::add_random_walk(uint8_t current_strip, uint dir, const CommandParams &cmd, CRGB color)
{
  Serial.println("Current " + String(current_strip) + " " + String(dir));

  int8_t next_edge = find_next_edge(current_strip);
  if (dir == WALK_DIR_DOWN)
  {
    next_edge = find_next_edge(current_strip * -1);
  }
  uint8_t next_dir = WALK_DIR_UP;
  if (next_edge < 0)
  {
    // next_dir = WALK_DIR_DOWN;
    next_edge = -next_edge;
  }

  effect_item_index++;
  if (effect_item_index >= EFFECT_ITEM_COUNT)
  {
    effect_item_index = 0;
  }
  EffectItem *new_ball = &effect_items[effect_item_index];

  new_ball->enabled = true;
  new_ball->color = color;
  new_ball->strip_index = next_edge;
  new_ball->command_index = cmd.index;
  new_ball->param_1 = next_dir;
  new_ball->param_2 = 0;
  new_ball->start_time = millis();
  new_ball->type = CommandParams_random_walk_tag;

  Serial.println("Start time " + String(new_ball->start_time));

  if (next_dir == WALK_DIR_DOWN)
  {
    new_ball->center = 59;
  }
  else
  {
    new_ball->center = 0;
  }
}

void Commands::random_walk(LedStructure *leds, const CommandParams &cmd)
{
  RandomWalk random_walk = cmd.type_params.random_walk;

  float brightness = float(cmd.brightness) / 255;
  float rate = float(random_walk.speed) / 10.0;

  uint8_t min_led = 0;
  uint8_t max_led = leds->strip_length(cmd.strip_index) - 1;

  uint32_t now = millis();

  CRGB color = color_from_palette(cmd, random_walk.color);

  uint8_t ballcount = 0;
  bool add_ball = false;
  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    if (effect_items[i].enabled && effect_items[i].command_index == cmd.index && effect_items[i].type == CommandParams_random_walk_tag)
    {
      ballcount++;
    }
  }
  if (ballcount == 0)
  {
    add_random_walk(1, WALK_DIR_UP, cmd, color);
  }

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
      float center = ball->center + distance;

      // generate new ball if out of bounds
      if (ball->param_2 == 0 && (center < min_led || center > max_led))
      {
        Serial.println("Next Edge. Center " + String(center) + " distance " + String(distance) + " start time " + String(ball->start_time) + " timediff " + String(time_diff));
        Serial.println("Effect index " + String(i));
        // add_random_walk(ball->strip_index, ball->param_1, cmd, color);
        ball->param_2 = 1;
      }

      // disable ball if out of bounds
      if (center > leds->strip_length(cmd.strip_index) + 60 || center < -60)
      {
        ball->enabled = false;
        Serial.println("Disable ball " + String(i));
      }
      else
      {
        render(leds, random_walk.shape, ball->strip_index, center, random_walk.width, ball->color, brightness, ball->param_1 == WALK_DIR_DOWN, false, false);
      }
    }
  }
}