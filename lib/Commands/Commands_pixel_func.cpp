#include <Commands.h>
#include <PixelFun2.h>
#include <WiFi.h>
#include <Network.h>
#include <ColorTools.h>
#include <libfixmath/fix16.h>

#define MAX_MILLIS 21600000 // 16 * 60 * 60 * 1000
// #define MAX_MILLIS 60000

PixelFun<256> pixelFun;
std::array<uint8_t, 4> current_hash = {0, 0, 0, 0};
std::array<uint8_t, 4> last_hash = {0, 0, 0, 0};

// colors
float min_distance = 5.0f;
float max_distance = 75.0f;
std::array<CRGB, 2> colors;

// transitions
uint32_t transition_interval_ms = 60 * 1000;
uint32_t transition_duration_ms = 1 * 1000;
bool in_transition = false;
uint32_t transition_start_time = 0;
std::array<CRGB, 2> start_colors;
std::array<CRGB, 2> target_colors;

void change_colors(PixelFunc pixel_func)
{
  if (in_transition)
  {
    if (millis() - transition_start_time > transition_duration_ms)
    {
      in_transition = false;
      colors = target_colors;
    }
    else
    {
      uint8_t progress = float(millis() - transition_start_time) / float(transition_duration_ms) * 255.0f + 0.5f;
      colors[0] = blend(start_colors[0], target_colors[0], progress);
      colors[1] = blend(start_colors[1], target_colors[1], progress);
    }
  }
  else
  {
    float distance = min_distance + float(pixel_func.color_distance) / 255.0f * (max_distance - min_distance);

    if (transition_start_time == 0)
    {
      colors = ColorTools::random_color_pair(distance);
      transition_start_time = millis();
    }

    if (millis() - transition_start_time > transition_interval_ms)
    {
      transition_start_time = millis();
      in_transition = true;

      start_colors = colors;
      target_colors = ColorTools::random_color_pair(distance);
    }
  }
}

void Commands::pixel_func(LedStructure *leds, CommandParams cmd)
{
  PixelFunc pixel_func = cmd.type_params.pixel_func;

  // only parse function if command hash has changed
  std::copy(std::begin(cmd.hash.bytes), std::end(cmd.hash.bytes), std::begin(current_hash));
  if (last_hash != current_hash)
  {
    last_hash = current_hash;
    pixelFun.parse(pixel_func.function);
  }

  change_colors(pixel_func);

  const uint32_t strip_length = leds->strip_length(cmd.strip_index);
  const fix16_t current_time = fix16_div(fix16_from_int(millis()), fix16_from_int(1000));

  for (int i = 0; i < leds->mapping_size(); i++)
  {
    const fix16_t x = fix16_from_float(leds->mapping_x(i));
    const fix16_t y = fix16_from_float(leds->mapping_y(i));
    const fix16_t index = (leds->mapping_sprip_index(i) - 1) * strip_length + leds->mapping_led(i);

    fix16_t pixelfun_value = pixelFun.eval(current_time, index, x, y);
    // pixelfun_value = fminf(fmaxf(pixelfun_value, -1.0f), 1.0f);

    if (send_debug && i == 10)
    {
      // Network::remote_log("x: " + String(fix16_to_float(x)) + " y: " + String(fix16_to_float(y)) + " current_time: " + String(fix16_to_float(current_time)) + " value: " + String(fix16_to_float(pixelfun_value)));
    }

    CRGB color;

    if (pixelfun_value < 0)
    {
      uint8_t scale = ease_out_cubic(-fix16_to_float(pixelfun_value)) * 255.0f;
      color = colors[0].scale8(scale);
    }
    else if (pixelfun_value > 0)
    {
      uint8_t scale = ease_out_cubic(fix16_to_float(pixelfun_value)) * 255.0f;
      color = colors[1].scale8(scale);
    }
    else
    {
      color = CRGB(0, 0, 0);
    }

    // color = color.scale8(cmd.brightness);
    ColorTools::gamma_correction(color);

    leds->fade_led(leds->mapping_sprip_index(i), leds->mapping_led(i), color, cmd.brightness);
  }
}
