#include <Commands.h>

void Commands::render_ball(uint8_t strip_index, int ball_type, float center, float width, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom) {
  switch(ball_type) {
    case BALL_TYPE_SQUARE:
      render_square_ball(strip_index, center, fabs(width), color, ball_brightness);
      break;
    case BALL_TYPE_SINE:
      render_sine_ball(strip_index, center, fabs(width), color, ball_brightness);
      break;
    case BALL_TYPE_COMET:
      render_comet(strip_index, center, width*10.0, color, ball_brightness, bounce_top, bounce_bottom);
      break;
    case BALL_TYPE_FILL_TOP:
      render_fill_top(strip_index, center, color, ball_brightness);
      break;
    case BALL_TYPE_FILL_BOTTOM:
      render_fill_bottom(strip_index, center, color, ball_brightness);
      break;
  }
}

void Commands::render_square_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));

  for (int i=start_led; i<=end_led; i++) {
    fade_led(strip_index, i, color, ball_brightness);
  }
}

void Commands::render_sine_ball(uint8_t strip_index, float center, float width, CRGB color, float ball_brightness) {
  int strip_length = strip_index_length(strip_index);
  int start_led = max(0, ceilf(center-width/2.0));
  int end_led = min(strip_length, floorf(center+width/2.0));
  float brightness = 0.0;

  for (int i=start_led; i<=end_led; i++) {
    brightness = 1.0 - fabs(float(i) - center) / (width / 2.0);
    Serial.print(brightness);
    Serial.print("-");
    brightness = ease_in_out_cubic(brightness);
    fade_led(strip_index, i, color, brightness);
  }
}

void Commands::render_comet(uint8_t strip_index, float center, float length, CRGB color, float ball_brightness, bool bounce_top, bool bounce_bottom) {
  
  render_sine_ball(strip_index, center, 3, color, ball_brightness);  

  if (length != 0) {
    float start;
    if (length > 0) {
      start = center - 0.5;
    } else {
      start = center + 0.5;
    }

    int strip_length = strip_index_length(strip_index);
    float end = start - length;
    float max_brightness = ball_brightness * 0.8;

    float slope = max_brightness / (start - end);

    int first_led = min(floorf(start), floorf(end));
    int last_led = max(ceilf(start), ceilf(end));

    for (int i=first_led; i<last_led; i++) {
      float brightness = ease_in_out_cubic(slope * (float(i) - end));
      if (brightness > 0) {
        int led_index = i;
        if (bounce_bottom && led_index < 0) {
          led_index = -1*led_index;
        }
        if (bounce_top && led_index >= strip_length) {
          led_index = 2*(strip_length)-led_index;
        }
        fade_led(strip_index, led_index, color, brightness);
      }
    }
  }
}

void Commands::render_fill_top(uint8_t strip_index, float center, CRGB color, float ball_brightness) {
  render_sine_ball(strip_index, center, 5, color, ball_brightness);  

  for (int i = center; i < strip_index_length(strip_index); i++) {
    fade_led(strip_index, i, color, ball_brightness);
  }
}

void Commands::render_fill_bottom(uint8_t strip_index, float center, CRGB color, float ball_brightness) {
  render_sine_ball(strip_index, center, 5, color, ball_brightness);  

  for (int i = 0; i < center; i++) {
    fade_led(strip_index, i, color, ball_brightness);
  }
}