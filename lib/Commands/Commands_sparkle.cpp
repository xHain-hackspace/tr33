#include <Commands.h>

#define MAX_SPARKLES 500
#define SPARKLES_DIM_RATE 40

uint8_t sparkle_index = 0;

struct Sparkle
{
  bool enabled;
  CRGB color;
  float width;
  float brightness;
  uint8_t strip_index;
  int center;
  int start_time;
};

Sparkle sparkles[MAX_SPARKLES];

void Commands::sparkle(Leds *leds, char *data)
{
  uint8_t strip_index = data[0];
  uint8_t color_index = random_or_value(data[1], 0, 255);
  float width = float(random_or_value(data[2], 0, 255)) / 10.0;
  uint8_t frequency = data[3]; // sparkles per seconds
  uint8_t duration = data[4];  // should never be 0
  int now = millis();

  static uint flicker_position;
  static uint flicker_count = 0;
  static uint flicker_width = 20;
  static uint flicker_maxwidth = 20;
  static uint flicker_delay = 1000; //ms
  static uint flicker_timestamp;
  static bool flicker_on = true;
  static uint flicker_stripindex = 0;

  flicker_on = data[5];
  flicker_delay = data[6] * 100;
  flicker_width = data[7];
  bool timesup = (now - flicker_timestamp) >= flicker_delay;

  if (duration == 0)
  {
    duration = 200;
  }

  if (flicker_on && flicker_count < 1 && timesup)
  {
    flicker_count = random(1, 5);
    flicker_width = random(1, min(int(flicker_maxwidth), leds->strip_length(sparkles[sparkle_index].strip_index) - 1));
    flicker_position = random(1, leds->strip_length(sparkles[sparkle_index].strip_index) - 1 - flicker_width);
    //sparkles[sparkle_index].strip_index = leds->random_strip(strip_index);
    flicker_stripindex = leds->random_strip(strip_index);
    flicker_timestamp = millis();
  }

  bool checkcase = flicker_on && timesup || (flicker_count > 0 && (frequency > 0 && (1000 / (now - sparkles[sparkle_index].start_time)) < frequency));
  if (checkcase || (!flicker_on && (frequency > 0 && (1000 / (now - sparkles[sparkle_index].start_time)) < frequency)))
  {
    if (sparkle_index++ >= MAX_SPARKLES)
    {
      sparkle_index = 0;
    }
    sparkles[sparkle_index].enabled = true;
    sparkles[sparkle_index].color = color_index == 255 ? COLOR_WHITE : ColorFromPalette(currentPalette, color_index);
    sparkles[sparkle_index].width = width;
    sparkles[sparkle_index].brightness = float(random(10)) / 20.0 + 0.5;

    if (flicker_on)
    {
      flicker_count--;
      sparkles[sparkle_index].center = flicker_position + random(0, flicker_width);
      sparkles[sparkle_index].strip_index = flicker_stripindex;
    }
    else
    {
      sparkles[sparkle_index].strip_index = leds->random_strip(strip_index);
      sparkles[sparkle_index].center = random(0, leds->strip_length(sparkles[sparkle_index].strip_index) - 1);
    }
    sparkles[sparkle_index].start_time = now;
  }

  for (int i = 0; i < MAX_SPARKLES; i++)
  {
    if (sparkles[i].enabled)
    {
      float brightness = Commands::ease_out_cubic(sparkles[i].brightness - float(now - sparkles[i].start_time) / (20.0 * float(duration)));
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