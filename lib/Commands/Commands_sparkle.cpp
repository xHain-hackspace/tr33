#include <Commands.h>

int last_sparkle_time[COMMAND_COUNT];
uint32_t fade_in_duration_ms = 200;

void Commands::sparkle(LedStructure *leds, CommandParams cmd)
{
  Sparkle sparkle = cmd.type_params.sparkle;

  float width = float(sparkle.sparkle_width) / 30.0;
  float frequency = float(sparkle.sparle_rate) * float(leds->pixel_count(cmd.strip_index)) / 1500.0; // sparkles per seconds
  float duration = max(1, sparkle.duration) * 100.0;

  int now = millis();
  CRGB color;

  // new sparkle
  if (frequency > 0 && (1000.0 / float(now - last_sparkle_time[cmd.index]) < frequency))
  {
    effect_item_index++;
    if (effect_item_index >= EFFECT_ITEM_COUNT)
    {
      effect_item_index = 0;
    }

    uint8_t new_color_index = 0;
    switch (sparkle.color_type)
    {
    case ColorType_SINGLE_COLOR:
      new_color_index = sparkle.color;
      break;
    case ColorType_RANDOM_COLOR:
      new_color_index = random(0, 255);
      break;
    }

    effect_items[effect_item_index].enabled = true;
    effect_items[effect_item_index].color_index = new_color_index;
    effect_items[effect_item_index].param_1 = random(0, 255);
    effect_items[effect_item_index].strip_index = leds->random_strip(cmd.strip_index);
    effect_items[effect_item_index].command_index = cmd.index;
    effect_items[effect_item_index].center = float(random(0, (leds->strip_length(effect_items[effect_item_index].strip_index) - 1)));
    effect_items[effect_item_index].start_time = now;

    last_sparkle_time[cmd.index] = now;
  }

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    if (effect_items[i].enabled && effect_items[i].command_index == cmd.index)
    {
      uint32_t time_diff = (now - effect_items[i].start_time);

      if (time_diff > duration)
      {
        effect_items[i].enabled = false;
      }
      else
      {
        float time_to_peak;

        if (time_diff < fade_in_duration_ms)
        {
          time_to_peak = float(time_diff) / float(fade_in_duration_ms);
        }
        else
        {
          time_to_peak = 1.0 - float(time_diff - fade_in_duration_ms) / duration;
        }

        float brightness_modifier = Commands::ease_out_cubic(float(effect_items[i].param_1) / 255.0);
        float brightness = Commands::ease_out_cubic(time_to_peak) * float(cmd.brightness) * brightness_modifier / 255;

        if (sparkle.color_type == ColorType_WHITE)
        {
          color = COLOR_WHITE;
        }
        else
        {
          color = color_from_palette(cmd, effect_items[i].color_index);
        }

        render_ball(leds, effect_items[i].strip_index, effect_items[i].center, width, color, brightness, false);
      }
    }
  }
}