#include <Commands.h>

int last_sparkle_time[COMMAND_COUNT];
uint32_t fade_in_duration_ms = 200;

void Commands::sparkle(LedStructure *leds, CommandParams cmd)
{
  Sparkle sparkle = cmd.type_params.sparkle;

  float width = float(max(1, sparkle.sparkle_width)) / 30.0;
  float frequency = float(sparkle.sparle_rate) * float(leds->pixel_count(cmd.strip_index)) / 1700.0; // sparkles per seconds
  float duration = max(1, sparkle.duration) * 100.0;
  float time_to_peak;

  int now = millis();

  // new sparkle
  if (frequency > 0 && (1000.0 / float(now - last_sparkle_time[cmd.index]) < frequency))
  {
    effect_item_index++;
    if (effect_item_index >= EFFECT_ITEM_COUNT)
    {
      effect_item_index = 0;
    }

    EffectItem *new_sparkle = &effect_items[effect_item_index];

    CRGB new_color;
    switch (sparkle.color_type)
    {
    case ColorType_SINGLE_COLOR:
      new_color = color_from_palette(cmd, sparkle.color);
      break;
    case ColorType_RANDOM_COLOR:
      new_color = color_from_palette(cmd, random(0, 255));
      break;
    case ColorType_WHITE:
      new_color = COLOR_WHITE;
      break;
    }

    new_sparkle->enabled = true;
    new_sparkle->color = new_color;
    new_sparkle->param_1 = random(0, 255); // initial brightness
    new_sparkle->strip_index = leds->random_strip(cmd.strip_index);
    new_sparkle->command_index = cmd.index;
    new_sparkle->center = float(random(0, (leds->strip_length(new_sparkle->strip_index) - 1)));
    new_sparkle->start_time = now;
    new_sparkle->type = CommandParams_sparkle_tag;

    last_sparkle_time[cmd.index] = now;
  }

  for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
  {
    EffectItem *sparkle_item = &effect_items[i];

    if (sparkle_item->enabled && sparkle_item->command_index == cmd.index && sparkle_item->type == CommandParams_sparkle_tag)
    {
      uint32_t time_diff = (now - sparkle_item->start_time);

      if (time_diff > duration)
      {
        sparkle_item->enabled = false;
      }
      else
      {

        if (time_diff < fade_in_duration_ms)
        {
          time_to_peak = float(time_diff) / float(fade_in_duration_ms);
        }
        else
        {
          time_to_peak = 1.0 - float(time_diff - fade_in_duration_ms) / duration;
        }

        float brightness_modifier = Commands::ease_out_cubic(float(sparkle_item->param_1) / 255.0);
        float brightness = Commands::ease_out_cubic(time_to_peak) * brightness_modifier * float(cmd.brightness) / 255.0;

        render_ball(leds, sparkle_item->strip_index, sparkle_item->center, width, sparkle_item->color, brightness, false);
      }
    }
  }
}