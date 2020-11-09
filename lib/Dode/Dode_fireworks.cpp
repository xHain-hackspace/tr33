#include <Dode.h>
#include <Commands.h>

#define MAX_SPARKS 64

struct Spark
{
  int8_t edge;
  float strength;
};

Spark sparks[MAX_SPARKS];
Spark next_sparks[MAX_SPARKS];
uint8_t active_sparks = 0;
float relative_position = 0.4;
long f_last_update = millis();

void Dode::fireworks(uint8_t *data)
{
  uint8_t color_index = data[0];
  float brightness = data[1];
  float rate = data[2];
  float width = float(data[3]) / 10;

  CRGB color = ColorFromPalette(currentPalette, color_index, 255);
  long now = millis();

  relative_position = relative_position + float(now - f_last_update) / 1000.0;

  if (relative_position > 1)
  {
    relative_position = 0;
    uint8_t new_spark_count = 0;

    for (int i = 0; i < active_sparks; i++)
    {
      if (random8(0, 3) == 0)
      {
        float strength = 1.0;
        if (sparks[i].edge > 0)
        {
          next_sparks[new_spark_count].edge = corners[sparks[i].edge - 1].top_left;
          next_sparks[new_spark_count + 1].edge = corners[sparks[i].edge - 1].top_right;
        }
        else
        {
          next_sparks[new_spark_count].edge = corners[abs8(sparks[i].edge) - 1].bottom_left;
          next_sparks[new_spark_count + 1].edge = corners[abs8(sparks[i].edge) - 1].bottom_right;
        }
        next_sparks[new_spark_count + 2].edge = sparks[i].edge * -1;

        next_sparks[new_spark_count].strength = strength;
        next_sparks[new_spark_count + 1].strength = strength;
        next_sparks[new_spark_count + 2].strength = strength;

        new_spark_count += 3;
      }
    }

    for (int i = 0; i < new_spark_count; i++)
    {
      sparks[i] = next_sparks[i];
    }
    active_sparks = new_spark_count;
  }

  for (int i = 0; i < active_sparks; i++)
  {
    render_ball(this, sparks[i].edge, relative_position * edge_strip_length(this, sparks[i].edge), width, color, brightness, RENDER_COMET);
  }

  if (active_sparks == 0)
  {
    sparks[0].edge = random_strip(EDGE_COUNT) + 1;
    sparks[0].strength = 1.0;
    active_sparks = 1;
  }

  f_last_update = now;
}
