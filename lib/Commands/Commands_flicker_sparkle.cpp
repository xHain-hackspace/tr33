#include <Commands.h>

#define FLICKER_SPARKLES_DIM_RATE 40

int last_flicker_sparkle[COMMAND_COUNT];

void Commands::flicker_sparkle(LedStructure *leds, CommandParams cmd)
{
    FlickerSparkle flicker_sparkle = cmd.type_params.flicker_sparkle;

    float width = float(flicker_sparkle.sparkle_width) / 10.0;
    uint8_t frequency = flicker_sparkle.sparles_per_second; // sparkles per seconds
    uint8_t duration = max(1, flicker_sparkle.duration);    // should never be 0
    uint16_t flicker_delay = flicker_sparkle.flicker_delay * 100;
    uint8_t flicker_width = flicker_sparkle.flicker_window;
    uint8_t flicker_maxflicker = max(1, flicker_sparkle.max_flickers);
    int now = millis();

    static uint flicker_position;
    static uint flicker_number = 0;
    static uint flicker_maxwidth = 20;
    static uint flicker_timestamp;
    static uint flicker_stripindex = 0;

    bool times_up = (now - flicker_timestamp) >= flicker_delay;

    if (flicker_number < 1 && times_up)
    {
        flicker_number = random(1, flicker_maxflicker);
        flicker_stripindex = leds->random_strip(cmd.strip_index);
        flicker_width = random(1, min(int(flicker_maxwidth), leds->strip_length(flicker_stripindex) - 1));
        flicker_position = random(1, leds->strip_length(flicker_stripindex) - flicker_width - 1);
        flicker_timestamp = millis();
    }

    if (times_up || (flicker_number > 0 && (frequency > 0 && (1000 / (now - last_flicker_sparkle[cmd.index]) < frequency))))
    {
        // new sparkles
        effect_item_index++;
        if (effect_item_index >= EFFECT_ITEM_COUNT)
        {
            effect_item_index = 0;
        }

        EffectItem *new_sparkle = &effect_items[effect_item_index];

        CRGB new_color;
        switch (flicker_sparkle.color_type)
        {
        case ColorType_SINGLE_COLOR:
            new_color = color_from_palette(cmd, flicker_sparkle.color);
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
        new_sparkle->strip_index = cmd.strip_index;
        new_sparkle->command_index = cmd.index;
        new_sparkle->param_1 = random(0, 255); // initial brightness
        new_sparkle->center = flicker_position + random(0, flicker_width);
        new_sparkle->start_time = now;
        new_sparkle->type = CommandParams_flicker_sparkle_tag;

        last_flicker_sparkle[cmd.index] = now;

        flicker_number--;
    }

    for (int i = 0; i < EFFECT_ITEM_COUNT; i++)
    {
        EffectItem *sparkle_item = &effect_items[i];

        if (sparkle_item->enabled && sparkle_item->command_index == cmd.index && sparkle_item->type == CommandParams_flicker_sparkle_tag)
        {

            uint32_t time_diff = (now - sparkle_item->start_time);

            if (time_diff > duration)
            {
                sparkle_item->enabled = false;
            }
            else
            {
                float brightness_modifier = Commands::ease_out_cubic(float(sparkle_item->param_1) / 255.0);
                float brightness = Commands::ease_out_cubic(1.0 - float(time_diff) / duration) * brightness_modifier * float(cmd.brightness) / 255.0;

                render_ball(leds, sparkle_item->strip_index, sparkle_item->center, width, sparkle_item->color, brightness, false);
            }
        }
    }
}