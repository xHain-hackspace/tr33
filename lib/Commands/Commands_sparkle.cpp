#include <Commands.h>

#define MAX_SPARKLES 500

uint8_t sparkle_index = 0;

struct SingleSparkle
{
  bool enabled;
  uint8_t strip_index;
  uint8_t command_index;
  CRGB color;
  int start_time;
  float width;
  float brightness;
  int center;
};

SingleSparkle sparkles[MAX_SPARKLES];

int last_sparkle[COMMAND_COUNT];

void Commands::sparkle(LedStructure *leds, CommandParams cmd)
{
  Sparkle sparkle = cmd.type_params.sparkle;

  float width = float(sparkle.sparkle_width) / 10.0;
  float frequency = float(sparkle.sparle_rate) * float(leds->pixel_count(cmd.strip_index)) / 1500.0; // sparkles per seconds
  uint8_t duration = max(1, sparkle.duration);

  int now = millis();

  if (frequency > 0 && (1000.0 / float(now - last_sparkle[cmd.index]) < frequency))
  {
    if (sparkle_index++ >= MAX_SPARKLES)
    {
      sparkle_index = 0;
    }
    sparkles[sparkle_index].enabled = true;
    sparkles[sparkle_index].color = sparkle.color == 255 ? COLOR_WHITE : color_from_palette(cmd, random_or_value(sparkle.color, 1, 255));
    sparkles[sparkle_index].width = width;
    sparkles[sparkle_index].brightness = (float(random(10)) / 20.0 + 0.5);
    sparkles[sparkle_index].strip_index = leds->random_strip(cmd.strip_index);
    sparkles[sparkle_index].center = random(0, leds->strip_length(sparkles[sparkle_index].strip_index) - 1);
    sparkles[sparkle_index].start_time = now;
    sparkles[sparkle_index].command_index = cmd.index;
    last_sparkle[cmd.index] = now;
  }

  for (int i = 0; i < MAX_SPARKLES; i++)
  {
    if (sparkles[i].enabled && sparkles[i].command_index == cmd.index)
    {
      float brightness = Commands::ease_out_cubic(sparkles[i].brightness - float(now - sparkles[i].start_time) / (20.0 * float(duration))) * float(cmd.brightness) / 255.0;
      if (brightness > 0)
      {
        render_ball(leds, sparkles[i].strip_index, sparkles[i].center, sparkles[i].width, sparkles[i].color, brightness);
      }
      else
      {
        sparkles[i].enabled = false;
      }
    }
  }
}