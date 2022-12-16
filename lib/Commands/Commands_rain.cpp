#include <Commands.h>

#define MAX_RAIN_DROPS 800

uint8_t drop_index = 0;

struct Drop
{
  bool enabled;
  uint8_t strip_index;
  uint8_t command_index;
  CRGB color;
  float width;
  float center;
  int start_time;
  // float rate;
};

Drop drops[MAX_RAIN_DROPS];

int drop_duration = 10000; // ms
int last_drop[COMMAND_COUNT];

void Commands::rain(LedStructure *leds, CommandParams cmd)
{
  Rain rain = cmd.type_params.rain;

  float width = float(rain.width) / 10.0;
  float frequency = float(rain.drop_density) * float(leds->pixel_count(cmd.strip_index)) / 2000.0; // drops per second (scaled with pixel count)
  float rate = float(rain.drop_speed) / 10.0;                                                      // drop speed

  int now = millis();

  if (frequency > 0 && (1000.0 / float(now - last_drop[cmd.index])) < frequency)
  {
    if (drop_index++ >= MAX_RAIN_DROPS)
    {
      drop_index = 0;
    }
    drops[drop_index].enabled = true;
    drops[drop_index].color = color_from_palette(cmd, random_or_value(rain.color, 0, 255));
    drops[drop_index].width = width;
    drops[drop_index].strip_index = leds->random_strip(cmd.strip_index);
    drops[drop_index].command_index = cmd.index;
    drops[drop_index].center = random(0, leds->strip_length(drops[drop_index].strip_index) + 20);
    drops[drop_index].start_time = now;
    last_drop[cmd.index] = now;
  }

  for (int i = 0; i < MAX_RAIN_DROPS; i++)
  {
    if (drops[i].enabled && drops[i].command_index == cmd.index)
    {
      int time_diff = (now - drops[i].start_time);
      float center = drops[i].center - float(time_diff) / 1000.0 * float(rate);
      float time_to_peak = 1.0 - fabs(time_diff - drop_duration / 2) / float(drop_duration / 2);
      float brightness = Commands::ease_out_cubic(time_to_peak) * float(cmd.brightness) / 255;

      if (time_diff > drop_duration || center < -10)
      {
        drops[i].enabled = false;
      }
      else
      {
        render(leds, Shape1D_COMET, drops[i].strip_index, center, drops[i].width, drops[i].color, brightness, false);
      }
    }
  }
}