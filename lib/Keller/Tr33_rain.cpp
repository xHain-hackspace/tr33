#include <Tr33.h>
#include <Commands.h>

uint8_t drop_index = 0;

struct Drop
{
  bool enabled;
  CRGB color;
  float width;
  uint8_t strip_index;
  float center;
  int start_time;
  float rate;
};

Drop drops[MAX_RAIN_DROPS];

int drop_duration = 7500; // ms

void Tr33::rain(char *data)
{
  uint8_t strip_index = data[0];
  uint8_t color_index = Commands::random_or_value(data[1], 0, 255);
  float width = float(Commands::random_or_value(data[2], 0, 255)) / 10.0;
  uint8_t frequency = data[3]; // drops per seconds
  float rate = float(Commands::random_or_value(data[4], 0, 255)) / 10.0;

  int now = millis();

  if (frequency > 0 && (1000 / (now - drops[drop_index].start_time)) < frequency)
  {
    if (drop_index++ >= MAX_RAIN_DROPS)
    {
      drop_index = 0;
    }
    drops[drop_index].enabled = true;
    drops[drop_index].color = color_index == 255 ? COLOR_WHITE : ColorFromPalette(currentPalette, color_index);
    drops[drop_index].width = width;
    drops[drop_index].strip_index = random_strip(strip_index);
    drops[drop_index].center = random(0, strip_length(drops[drop_index].strip_index) - 1);
    drops[drop_index].start_time = now;
    drops[drop_index].rate = rate;
  }

  for (int i = 0; i < MAX_RAIN_DROPS; i++)
  {
    if (drops[i].enabled)
    {
      int time_diff = (now - drops[i].start_time);
      float center = drops[i].center - float(time_diff) / 1000.0 * float(drops[i].rate);
      float time_to_peak = 1.0 - fabs(time_diff - drop_duration / 2) / float(drop_duration / 2);
      float brightness = Commands::ease_out_cubic(time_to_peak);

      if (time_diff > drop_duration || center < -10)
      {
        drops[i].enabled = false;
      }
      else
      {
        render_ball(drops[i].strip_index, BALL_TYPE_COMET, center, -1.0 * drops[i].width, drops[i].color, brightness, false, false);
      }
    }
  }
}