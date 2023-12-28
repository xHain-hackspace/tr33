#include <Commands.h>
#include <Modifiers.h>

#define FADE_OUT_ITEM_COUNT 20

struct FadeOutItem
{
  bool enabled;
  CRGB color;
  uint32_t start_time;
  uint8_t strip_index;
  uint8_t center;
  uint8_t decay;
  uint8_t brightness;
  uint8_t shape;
  uint8_t width;
};

u_int32_t fade_out_item_index = 0;
FadeOutItem fade_out_items[FADE_OUT_ITEM_COUNT];

ColorPalette color_palette = ColorPalette_RAINBOW;

void Commands::fade_out_event(LedStructure *leds, const FadeOutEvent &event)
{
  fade_out_item_index++;
  if (fade_out_item_index >= FADE_OUT_ITEM_COUNT)
  {
    fade_out_item_index = 0;
  }
  FadeOutItem *new_item = &fade_out_items[fade_out_item_index];

  new_item->enabled = true;
  new_item->color = color_from_palette(color_palette, event.color, 255);
  new_item->strip_index = event.strip_index;
  new_item->start_time = millis();
  new_item->center = event.position;
  new_item->decay = event.decay;
  new_item->brightness = event.brightness;
  new_item->shape = event.shape;
  new_item->width = event.width;
}

void Commands::fade_out(LedStructure *leds, const CommandParams &cmd)
{
  FadeOut fade_out = cmd.type_params.fade_out;
  color_palette = cmd.color_palette;

  uint32_t now = millis();

  for (int i = 0; i < FADE_OUT_ITEM_COUNT; i++)
  {
    FadeOutItem *item = &fade_out_items[i];

    if (item->enabled)
    {
      uint32_t time_diff = (now - item->start_time);
      float brightness = float(item->brightness) / 255.0f - float(time_diff) / 1000.0f * float(item->decay) / 10.0f;

      if (brightness <= 0)
      {
        item->enabled = false;
      }
      else
      {
        render(leds, (Shape1D)item->shape, item->strip_index, item->center, item->width, item->color, brightness, false, false, false);
      }
    }
  }
}