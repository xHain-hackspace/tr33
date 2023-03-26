#include <Commands.h>

float drop_duration = 10000; // ms
uint32_t last_drop_time[COMMAND_COUNT];

void Commands::rain(LedStructure *leds, CommandParams cmd)
{
  Rain rain = cmd.type_params.rain;

  float width = float(rain.width) / 8.0;
  float frequency = float(rain.drop_density) * float(leds->pixel_count(cmd.strip_index)) / 2500.0; // drops per second (scaled with pixel count)
  float rate = float(rain.drop_speed) / 10.0;                                                      // drop speed

  uint32_t now = millis();
  CRGB color;

  if (frequency > 0 && (1000.0 / float(max((uint32_t)1, now - last_drop_time[cmd.index]))) < frequency)
  {
    effect_item_index++;
    if (effect_item_index >= EFFECT_ITEM_COUNT)
    {
      effect_item_index = 0;
    }

    uint8_t new_color_index = 0;
    switch (rain.color_type)
    {
    case ColorType_SINGLE_COLOR:
      new_color_index = rain.color;
      break;
    case ColorType_RANDOM_COLOR:
      new_color_index = random(0, 255);
      break;
    }

    effect_items[effect_item_index].enabled = true;
    effect_items[effect_item_index].color_index = new_color_index;
    effect_items[effect_item_index].strip_index = leds->random_strip(cmd.strip_index);
    effect_items[effect_item_index].command_index = cmd.index;
    effect_items[effect_item_index].center = random(0, leds->strip_length(effect_items[effect_item_index].strip_index) + leds->strip_length(effect_items[effect_item_index].strip_index) / 4);
    effect_items[effect_item_index].start_time = now;

    last_drop_time[cmd.index] = now;
  }

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    if (effect_items[i].enabled && effect_items[i].command_index == cmd.index)
    {
      uint32_t time_diff = (now - effect_items[i].start_time);
      float center = effect_items[i].center - float(time_diff) / 1000.0 * float(rate);

      if (time_diff > drop_duration || center < -10)
      {
        effect_items[i].enabled = false;
      }
      else
      {
        float time_to_peak = 1.0 - fabs(time_diff - drop_duration / 2) / float(drop_duration / 2);
        float brightness = Commands::ease_out_cubic(time_to_peak) * float(cmd.brightness) / 255;

        if (rain.color_type == ColorType_WHITE)
        {
          color = COLOR_WHITE;
        }
        else
        {
          color = color_from_palette(cmd, effect_items[i].color_index);
        }

        render(leds, Shape1D_COMET, effect_items[i].strip_index, center, width, color, brightness, false, false, false);
      }
    }
  }
}