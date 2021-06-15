#include <Commands.h>

#define MAPPING_CAM_PX_PER_M 144.0
#define PARTICLES_MAX_PARTICLES 5

struct Particle
{
  float x;   //map coordinates
  float y;   //map coordinates
  float v_x; //map coordinates/s
  float v_y; //map coordinates/s
  float m;   //kg
  CRGB color;
};

Particle particles[PARTICLES_MAX_PARTICLES];

long particles_last_update = 0;

void Commands::mapped_particles(LedStructure *leds, CommandParams cmd)
{
  MappedParticles mapped_particles = cmd.type_params.mapped_particles;

  //parameters
  float render_brightness = float(cmd.brightness) / 255.0;
  float x_start = (MAPPING_X_MAX - MAPPING_X_MIN) * float(mapped_particles.x) / 255.0 + MAPPING_X_MIN;
  float y_start = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(255 - mapped_particles.y) / 255.0 + MAPPING_Y_MIN;
  float size = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(mapped_particles.size) / 255.0;
  float fade_distance = (MAPPING_Y_MAX - MAPPING_Y_MIN) * float(mapped_particles.fade_distance) * 0.2 / 255.0;

  CRGB color = color_from_palette(cmd, mapped_particles.color);

  //time
  long now = millis();
  float delta_t = (now - particles_last_update) / 1000.0; //in seconds
  particles_last_update = now;

  for (int i = 0; i < PARTICLES_MAX_PARTICLES; i++)
  {
    if ((particles[i].x < (MAPPING_X_MIN - 0.5 * (MAPPING_X_MAX - MAPPING_X_MIN))) || (particles[i].x > (MAPPING_X_MAX + 0.5 * (MAPPING_X_MAX - MAPPING_X_MIN))) || (particles[i].y < (MAPPING_Y_MIN - 0.5 * (MAPPING_Y_MAX - MAPPING_Y_MIN))) || (particles[i].y > (MAPPING_Y_MAX + 0.5 * (MAPPING_Y_MAX - MAPPING_Y_MIN))))
    {
      particles[i].color = color;
      particles[i].x = x_start;
      particles[i].y = y_start;
      particles[i].v_x = random(-30, 30) * 10.0;
      particles[i].v_y = random(-20, -10) * 10.0;
      particles[i].m = 0.1;
    }

    //force
    float f_x = 0.0; //Newton
    float f_y = 10;  //Newton

    //speed change
    float delta_v_x = f_x / particles[i].m * (float)delta_t;
    float delta_v_y = f_y / particles[i].m * (float)delta_t;

    //distance change
    float delta_s_x = particles[i].v_x * (float)delta_t;
    float delta_s_y = particles[i].v_y * (float)delta_t;

    //update absolute speed/distance
    particles[i].v_x += delta_v_x;
    particles[i].v_y += delta_v_y;
    particles[i].x += delta_s_x;
    particles[i].y += delta_s_y;

    mapped_render_ball(leds, particles[i].x, particles[i].y, size, particles[i].color, render_brightness, fade_distance);
  }
}