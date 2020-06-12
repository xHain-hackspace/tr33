#include <Commands.h>

#define MAX_FLICKER_SPARKLES 500
#define FLICKER_SPARKLES_DIM_RATE 40

uint8_t flicker_sparkle_index = 0;

struct Flicker_Sparkle
{
    bool enabled;
    CRGB color;
    float width;
    float brightness;
    uint8_t strip_index;
    int center;
    int start_time;
};

Flicker_Sparkle flicker_sparkles[MAX_FLICKER_SPARKLES];

void Commands::flicker_sparkle(LedStructure *leds, char *data)
{
    uint8_t strip_index = data[0];
    uint8_t color_index = random_or_value(data[1], 0, 255);
    float width = float(random_or_value(data[2], 0, 255)) / 10.0;
    uint8_t frequency = data[3]; // sparkles per seconds
    uint8_t duration = data[4];  // should never be 0
    uint16_t flicker_delay = data[5] * 100;
    uint8_t flicker_width = data[6];
    uint8_t flicker_maxflicker = data[7];
    int now = millis();

    static uint flicker_position;
    static uint flicker_number = 0;
    static uint flicker_maxwidth = 20;
    static uint flicker_timestamp;
    static uint flicker_stripindex = 0;

    bool times_up = (now - flicker_timestamp) >= flicker_delay;

    if (duration == 0)
    {
        duration = 200;
    }

    if (flicker_maxflicker == 0)
    {
        flicker_maxflicker = 1;
    }

    if (flicker_number < 1 && times_up)
    {
        flicker_number = random(1, flicker_maxflicker);
        flicker_width = random(1, min(int(flicker_maxwidth), leds->strip_length(flicker_sparkles[flicker_sparkle_index].strip_index) - 1));
        flicker_position = random(1, leds->strip_length(flicker_sparkles[flicker_sparkle_index].strip_index) - flicker_width - 1);
        flicker_stripindex = leds->random_strip(strip_index);
        flicker_timestamp = millis();
    }

    if (times_up || (flicker_number > 0 && (frequency > 0 && (1000 / (now - flicker_sparkles[flicker_sparkle_index].start_time)) < frequency)))
    {
        if (flicker_sparkle_index++ >= MAX_FLICKER_SPARKLES)
        {
            flicker_sparkle_index = 0;
        }
        flicker_sparkles[flicker_sparkle_index].enabled = true;
        flicker_sparkles[flicker_sparkle_index].color = color_index == 255 ? COLOR_WHITE : ColorFromPalette(currentPalette, color_index);
        flicker_sparkles[flicker_sparkle_index].width = width;
        flicker_sparkles[flicker_sparkle_index].brightness = float(random(10)) / 20.0 + 0.5;

        flicker_number--;
        flicker_sparkles[flicker_sparkle_index].center = flicker_position + random(0, flicker_width);
        flicker_sparkles[flicker_sparkle_index].strip_index = flicker_stripindex;

        flicker_sparkles[flicker_sparkle_index].start_time = now;
    }

    for (int i = 0; i < MAX_FLICKER_SPARKLES; i++)
    {
        if (flicker_sparkles[i].enabled)
        {
            float brightness = Commands::ease_out_cubic(flicker_sparkles[i].brightness - float(now - flicker_sparkles[i].start_time) / (20.0 * float(duration)));
            if (brightness > 0)
            {
                render_ball(leds, flicker_sparkles[i].strip_index, flicker_sparkles[i].center, flicker_sparkles[i].width, flicker_sparkles[i].color, brightness);
            }
            else
            {
                flicker_sparkles[i].enabled = false;
            }
        }
    }
}