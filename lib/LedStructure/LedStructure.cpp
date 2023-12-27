#include <Commands.h>
#include <LedStructure.h>
#include <command_schemas.pb.h>

CRGB LedStructure::leds[STRIP_COUNT][STRIP_PIXEL_COUNT];

void LedStructure::init()
{
  // Modifier modifer = Modifier_init_default;
  // modifer.movement_type = MovementType_RANDOM_TRANSITIONS;
  // modifer.period_s = 10;

  // Rainbow rainbow = Rainbow_init_default;
  // commands[0] = CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_rainbow_tag;
  // commands[0].type_params.rainbow = rainbow;
  // commands[0].color_palette = ColorPalette_SCOUTY;

  // SingleColor single = SingleColor_init_default;
  // single.color = HUE_BLUE;
  // commands[0] = CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_single_color_tag;
  // commands[0].type_params.single_color = single;

  // modifer.movement_type = MovementType_LINEAR;
  // modifer.beats_per_minute =  256;
  // commands[0].color_palette_modifier = modifer;
  // commands[0].has_color_palette_modifier = true;

  // Rain rain = Rain_init_default;
  // rain.drop_density = 10;
  // rain.width = 50;
  // rain.drop_speed = 50;
  // rain.color = HUE_PURPLE;
  // commands[0] = CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_rain_tag;
  // commands[0].type_params.rain = rain;
  // commands[0].brightness = 130;

  // PingPong ping_pong = PingPong_init_default;
  // ping_pong.color = HUE_GREEN;
  // ping_pong.shape = Shape1D_NYAN_BOUNCE;
  // ping_pong.width = 80;
  // ping_pong.period_100ms = 200;
  // commands[1] = CommandParams_init_default;
  // commands[1].which_type_params = CommandParams_ping_pong_tag;
  // commands[1].type_params.ping_pong = ping_pong;

  // BeatEqualizer be = BeatEqualizer_init_default;
  // be.color = HUE_GREEN;
  // be.band = 3;
  // commands[0] = CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_beat_equalizer_tag;
  // commands[0].type_params.beat_equalizer = be;

  Sparkle sparkle = Sparkle_init_default;
  sparkle.color = 0;
  sparkle.sparle_rate = 15;
  sparkle.duration = 150;
  sparkle.color_type = ColorType_RANDOM_COLOR;
  commands[0] = (CommandParams)CommandParams_init_default;
  commands[0].which_type_params = CommandParams_sparkle_tag;
  commands[0].type_params.sparkle = sparkle;
  commands[0].color_palette = ColorPalette_ORANGERY;
  commands[0].hash.size = 1;
  commands[0].hash.bytes[0] = 23;

  // Debug debug = Debug_init_default;

  // commands[0] = (CommandParams)CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_debug_tag;
  // commands[0].type_params.debug = debug;
  // commands[0].hash.size = 1;
  // commands[0].hash.bytes[0] = 23;

  // String init_program = String("sin(2*t-hypot(x-5,y-5))");
  // String init_program = String("x/16");
  // PixelFunc pixel_func = PixelFunc_init_default;
  // init_program.toCharArray(pixel_func.function, 40);
  // pixel_func.function_hash = 1;
  // commands[0] = (CommandParams)CommandParams_init_default;
  // commands[0].which_type_params = CommandParams_pixel_func_tag;
  // commands[0].type_params.pixel_func = pixel_func;
}

//
// -- Set leds ----------------------------------------
//

void LedStructure::set_led(uint8_t strip_index, int led, CRGB color)
{
  if (strip_index == 0)
  {
    for (int i = 0; i < STRIP_COUNT; i++)
    {
      leds[i][led] = color;
    }
  }
  else if (strip_index <= STRIP_COUNT)
  {
    leds[strip_index - 1][led] = color;
  }
}

CRGB LedStructure::get_led(uint8_t strip_index, int led)
{
  if (strip_index == 0)
  {
    return leds[0][led];
  }
  else if (strip_index <= STRIP_COUNT)
  {
    return leds[strip_index - 1][led];
  }
  else
  {
    return CRGB::Black;
  }
}

void LedStructure::fade_led(CommandParams cmd, int led, CRGB target)
{
  fade_led(cmd.strip_index, led, target, cmd.brightness);
}

void LedStructure::fade_led(uint8_t strip_index, int led, CRGB target, fract8 amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, target, amount);
    set_led(strip_index, led, faded);
  }
}

void LedStructure::shift_led(uint8_t strip_index, int led, uint8_t amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CRGB current = get_led(strip_index, led);
    CRGB faded = blend(current, 0, amount * 255.0);
    set_led(strip_index, led, faded);
  }
}

void LedStructure::invert_led(uint8_t strip_index, int led, fract8 amount)
{
  if (led >= 0 && led < strip_length(strip_index))
  {
    CHSV current = rgb2hsv_approximate(get_led(strip_index, led));
    CHSV target = current;
    target.hue = (current.hue + 128 % 256);
    CRGB blended = blend(current, target, amount);
    set_led(strip_index, led, blended);
  }
}

uint16_t LedStructure::strip_length(uint8_t strip_index)
{
  return STRIP_PIXEL_COUNT;
}

uint16_t LedStructure::pixel_count(uint8_t strip_index)
{
  if (strip_index == 0)
  {
    return STRIP_PIXEL_COUNT * STRIP_COUNT;
  }
  else
  {
    return STRIP_PIXEL_COUNT;
  }
}

uint8_t LedStructure::random_strip(uint8_t strip_index)
{
  if (strip_index == 0)
  {
    return random8(0, STRIP_COUNT);
  }
  else
  {
    return strip_index;
  }
}

uint8_t LedStructure::mapping_sprip_index(uint16_t index)
{
  return pgm_read_float(&mapping[index][0]);
}

int LedStructure::mapping_led(uint16_t index)
{
  return pgm_read_float(&mapping[index][1]);
}

static float mapping_x_range = MAPPING_X_MAX - MAPPING_X_MIN;
static float mapping_y_range = MAPPING_Y_MAX - MAPPING_Y_MIN;
static float scale = max(mapping_x_range, mapping_y_range) / 8.0f;

float LedStructure::mapping_x(uint16_t index)
{
  float x = pgm_read_float(&mapping[index][2]);
  x = (x - float(MAPPING_X_MIN)) / scale;
  return x;

  // go back to this once all mappings are scaled to 8
  // return pgm_read_float(&mapping[index][2]);
}

float LedStructure::mapping_y(uint16_t index)
{
  float y = pgm_read_float(&mapping[index][3]);
  y = (y - float(MAPPING_Y_MIN)) / scale;
  return y;

  // go back to this once all mappings are scaled to 8
  // return pgm_read_float(&mapping[index][3]);
}

uint16_t LedStructure::mapping_size()
{
  return MAPPING_SIZE;
}

FairyLightsControl *LedStructure::get_fairy_light(uint8_t index)
{
  return nullptr;
}

String LedStructure::get_name()
{
  return "Generic Led Structure";
}

uint8_t LedStructure::get_max_strip_index()
{
  if (STRIP_COUNT == 1)
  {
    return 0;
  }
  else
  {
    return STRIP_COUNT;
  }
}
