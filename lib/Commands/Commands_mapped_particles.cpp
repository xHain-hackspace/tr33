#include <Commands.h>

struct Particle
{
  float x;   //map coordinates
  float y;   //map coordinates
  float v_x; //map coordinates/s
  float v_y; //map coordinates/s
  float m;   //kg
};

Particle particles[2];

long particles_last_update = 0;

void Commands::mapped_particles(LedStructure *leds, char *data)
{
  //parameters
  uint8_t color_index = data[0];
  float render_brightness = float(data[1]) / 255.0;
  uint8_t shape = data[2];
  float x_start = (MAPPING_X_MAX - MAPPING_X_MIN) * float(data[3]) / 255.0 + MAPPING_X_MIN;
  float y_start = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - data[4]) / 255.0 + MAPPING_Y_MIN;
  float size = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(data[5]) / 255.0;
  float fade_distance = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(data[6]) / 255.0 / 0.5;

  CRGB color = ColorFromPalette(currentPalette, color_index);

  if ((particles[0].x < MAPPING_X_MIN) || (particles[0].x > MAPPING_X_MAX) || (particles[0].y < MAPPING_X_MIN) || (particles[0].y > MAPPING_X_MAX))
  {
    particles[0].x = x_start;
    particles[0].y = y_start;
    particles[0].v_x = 0.0;
    particles[0].v_y = 0.0;
    particles[0].m = 0.01;
  }

  //time
  long now = millis();
  long delta_t = now - particles_last_update;
  particles_last_update = now;

  //force
  float f_x = 0.0;
  float f_y = 9.81;

  //speed change
  float delta_v_x = f_x / particles[0].m * (float)delta_t;
  float delta_v_y = f_y / particles[0].m * (float)delta_t;

  //distance change
  float delta_s_x = particles[0].v_x * (float)delta_t;
  float delta_s_y = particles[0].v_y * (float)delta_t;

  //update absolute speed/distance
  particles[0].v_x += delta_v_x;
  particles[0].v_y += delta_v_y;
  particles[0].x += delta_s_x;
  particles[0].y += delta_s_y;

  mapped_render_ball(leds, x_start, y_start, size, color, render_brightness, fade_distance);
}