#include <Commands.h>

#define MAX_FLICKER_SPARKLES 200
#define FLICKER_SPARKLES_DIM_RATE 40

uint8_t flicker_sparkle_index = 0;

struct Flicker_Sparkle
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

Flicker_Sparkle flicker_sparkles[MAX_FLICKER_SPARKLES];

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
        flicker_width = random(1, min(int(flicker_maxwidth), leds->strip_length(flicker_sparkles[flicker_sparkle_index].strip_index) - 1));
        flicker_position = random(1, leds->strip_length(flicker_sparkles[flicker_sparkle_index].strip_index) - flicker_width - 1);
        flicker_stripindex = leds->random_strip(cmd.strip_index);
        flicker_timestamp = millis();
    }

    if (times_up || (flicker_number > 0 && (frequency > 0 && (1000 / (now - last_flicker_sparkle[cmd.index]) < frequency))))
    {
        if (flicker_sparkle_index++ >= MAX_FLICKER_SPARKLES)
        {
            flicker_sparkle_index = 0;
        }
        flicker_sparkles[flicker_sparkle_index].enabled = true;
        flicker_sparkles[flicker_sparkle_index].color = flicker_sparkle.color == 255 ? COLOR_WHITE : color_from_palette(cmd, random_or_value(flicker_sparkle.color, 1, 255));
        flicker_sparkles[flicker_sparkle_index].width = width;
        flicker_sparkles[flicker_sparkle_index].brightness = float(random(10)) / 20.0 + 0.5;

        flicker_number--;
        flicker_sparkles[flicker_sparkle_index].center = flicker_position + random(0, flicker_width);
        flicker_sparkles[flicker_sparkle_index].strip_index = flicker_stripindex;
        flicker_sparkles[flicker_sparkle_index].command_index = cmd.index;
        flicker_sparkles[flicker_sparkle_index].start_time = now;
        last_flicker_sparkle[cmd.index] = now;
    }

    for (int i = 0; i < MAX_FLICKER_SPARKLES; i++)
    {
        if (flicker_sparkles[i].enabled && flicker_sparkles[i].command_index == cmd.index)
        {
            float brightness = Commands::ease_out_cubic(flicker_sparkles[i].brightness - float(now - flicker_sparkles[i].start_time) / (20.0 * float(duration)));
            if (brightness > 0)
            {
                render_ball(leds, flicker_sparkles[i].strip_index, flicker_sparkles[i].center, flicker_sparkles[i].width, flicker_sparkles[i].color, brightness * float(cmd.brightness) / 255.0);
            }
            else
            {
                flicker_sparkles[i].enabled = false;
            }
        }
    }
}