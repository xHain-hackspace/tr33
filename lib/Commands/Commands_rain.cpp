#include <Commands.h>

float drop_duration = 10000; // ms
uint32_t last_drop_time[COMMAND_COUNT];

void Commands::rain(LedStructure *leds, const CommandParams &cmd)
{
  Rain rain = cmd.type_params.rain;

  float width = float(rain.width) / 8.0;
  float frequency = float(rain.drop_density) * float(leds->pixel_count(cmd.strip_index)) / 2500.0; // drops per second (scaled with pixel count)
  float rate = float(rain.drop_speed) / 10.0;                                                      // drop speed

  uint32_t now = millis();

  if (frequency > 0 && (1000.0 / float(max((uint32_t)1, now - last_drop_time[cmd.index]))) < frequency)
  {
    effect_item_index++;
    if (effect_item_index >= EFFECT_ITEM_COUNT)
    {
      effect_item_index = 0;
    }

    EffectItem *new_drop = &effect_items[effect_item_index];

    CRGB new_color;
    switch (rain.color_type)
    {
    case ColorType_SINGLE_COLOR:
      new_color = color_from_palette(cmd, rain.color);
      break;
    case ColorType_RANDOM_COLOR:
      new_color = color_from_palette(cmd, random(0, 255));
      break;
    case ColorType_WHITE:
      new_color = COLOR_WHITE;
      break;
    }

    new_drop->enabled = true;
    new_drop->color = new_color;
    new_drop->strip_index = leds->random_strip(cmd.strip_index);
    new_drop->command_index = cmd.index;
    new_drop->center = random(0, leds->strip_length(new_drop->strip_index) + leds->strip_length(new_drop->strip_index) / 4);
    new_drop->start_time = now;
    new_drop->type = CommandParams_rain_tag;

    last_drop_time[cmd.index] = now;
  }

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    EffectItem *drop = &effect_items[i];

    if (drop->enabled && drop->command_index == cmd.index && drop->type == CommandParams_rain_tag)
    {
      uint32_t time_diff = (now - drop->start_time);
      float center = drop->center - float(time_diff) / 1000.0 * float(rate);

      if (time_diff > drop_duration || center < -10)
      {
        drop->enabled = false;
      }
      else
      {
        float time_to_peak = 1.0 - fabs(time_diff - drop_duration / 2) / float(drop_duration / 2);
        float brightness = Commands::ease_out_cubic(time_to_peak) * float(cmd.brightness) / 255;

        render(leds, Shape1D_COMET, drop->strip_index, center, width, drop->color, brightness, false, false, false);
      }
    }
  }
}