#include <Tr33.h>
#include <Tr33_mapped.h>
#include <Commands.h>

float fraction = 0.0;
long last_mapped_update = millis();

bool render(int i, float fraction, uint8_t swipe_direction)
{
  if (swipe_direction == SWIPE_TOP_BOTTOM && mapping[i][3] < (Y_MAX - Y_MIN) * fraction + Y_MIN)
  {
    return true;
  }
  else if (swipe_direction == SWIPE_BOTTOM_TOP && mapping[i][3] > (Y_MAX - Y_MIN) * (1.0 - fraction) + Y_MIN)
  {
    return true;
  }
  else if (swipe_direction == SWIPE_LEFT_RIGHT && mapping[i][2] < (X_MAX - X_MIN) * fraction + X_MIN)
  {
    return true;
  }
  else if (swipe_direction == SWIPE_RIGHT_LEFT && mapping[i][2] > (X_MAX - X_MIN) * (1.0 - fraction) + X_MIN)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Tr33::mapped_swipe(char *data)
{
  uint8_t swipe_direction = data[0];
  uint8_t color_index = data[1];
  float rate = 1.0 + float(data[2]) / 10;

  CRGB color = ColorFromPalette(currentPalette, color_index);

  fraction += float(millis() - last_mapped_update) / 1000.0 / rate;
  if (fraction > 1)
  {
    fraction = 0;
  }

  for (int i = 0; i < MAPPING_SIZE; i++)
  {
    if (render(i, fraction, swipe_direction))
    {
      set_led(int(mapping[i][0]), int(mapping[i][1]), color);
    }
  }
  last_mapped_update = millis();
}

void Tr33::mapped_shape(char *data)
{
  uint8_t shape = data[0];
  uint8_t color_index = data[1];
  uint8_t x = data[2];
  uint8_t y = data[3];
  uint8_t size = data[4];

  CRGB color = ColorFromPalette(currentPalette, color_index);

  float x_mapped = X_MIN + (X_MAX - X_MIN) * (x / 255.0);
  float y_mapped = Y_MIN + (Y_MAX - Y_MIN) * (y / 255.0);
  float size_mapped = (Y_MAX - Y_MIN) * (size / 255.0);

  switch (shape)
  {
  case SHAPE_SQUARE:
    render_mapped_square(x_mapped, y_mapped, size_mapped, color, true);
    break;
  case SHAPE_HOLLOW_SQUARE:
    render_mapped_square(x_mapped, y_mapped, size_mapped, color, false);
    break;
    //   break;
    // case SHAPE_CIRCLE:
    //   render_mapped_circle(x_mapped, y_mapped, size_mapped, color);
    //   break;
  }
}

bool is_inside_square(float x, float y, float x_min, float x_max, float y_min, float y_max)
{
  return x > x_min && x < x_max && y > y_min && y < y_max;
}

void Tr33::render_mapped_square(float x, float y, float size, CRGB color, bool fill)
{
  float x_min = x - size / 2.0;
  float x_max = x + size / 2.0;
  float y_min = y - size / 2.0;
  float y_max = y + size / 2.0;

  float border_width = (Y_MAX - Y_MIN) / 75.0;

  for (int i = 0; i < MAPPING_SIZE; i++)
  {

    if (fill && is_inside_square(mapping[i][2], mapping[i][3], x_min, x_max, y_min, y_max))
    {
      set_led(int(mapping[i][0]), int(mapping[i][1]), color);
    }
    else if (!fill && is_inside_square(mapping[i][2], mapping[i][3], x_min, x_max, y_min, y_max) && !is_inside_square(mapping[i][2], mapping[i][3], x_min + border_width, x_max - border_width, y_min + border_width, y_max - border_width))
    {
      set_led(int(mapping[i][0]), int(mapping[i][1]), color);
    }
  }
}

void Tr33::render_mapped_circle(float x, float y, float size, CRGB color, bool fuzz)
{
  float brightness = 0.0;
  float distance = 0.0;

  for (int i = 0; i < MAPPING_SIZE; i++)
  {
    distance = sqrtf(powf(fabs(x - mapping[i][2]), 2) + powf(fabs(y - mapping[i][3]), 2));

    if (distance <= size)
    {
      if (fuzz)
      {
        brightness = 1.0 - distance / size;
        brightness = Commands::ease_in_out_cubic(brightness);
        fade_led(mapping[i][0], mapping[i][1], color, brightness);
      }
      else
      {
        set_led(int(mapping[i][0]), int(mapping[i][1]), color);
      }
    }
  }
}
