#include <Modifiers.h>

#define MIN_INDEX 3

int *Modifiers::value_from_type_param(CommandParams *cmd, uint8_t field_index)
{
  switch (cmd->which_type_params)
  {
  
  case CommandParams_white_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.white.color_temperature;
    
    }
  
  case CommandParams_single_color_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.single_color.color;
    
    }
  
  case CommandParams_pixel_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.pixel.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.pixel.led_index;
    
    }
  
  case CommandParams_pixel_rgb_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.pixel_rgb.red;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.pixel_rgb.green;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.pixel_rgb.blue;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.pixel_rgb.led_index;
    
    }
  
  case CommandParams_rainbow_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.rainbow.speed;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.rainbow.wave_size;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.rainbow.rainbow_size;
    
    }
  
  case CommandParams_sparkle_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.sparkle.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.sparkle.sparkle_width;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.sparkle.sparle_rate;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.sparkle.duration;
    
    }
  
  case CommandParams_flicker_sparkle_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.flicker_sparkle.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.flicker_sparkle.sparkle_width;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.flicker_sparkle.sparles_per_second;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.flicker_sparkle.duration;
    
    case MIN_INDEX + 4:
      return &cmd->type_params.flicker_sparkle.flicker_delay;
    
    case MIN_INDEX + 5:
      return &cmd->type_params.flicker_sparkle.flicker_window;
    
    case MIN_INDEX + 6:
      return &cmd->type_params.flicker_sparkle.max_flickers;
    
    }
  
  case CommandParams_ping_pong_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.ping_pong.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.ping_pong.width;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.ping_pong.count;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.ping_pong.period_100ms;
    
    }
  
  case CommandParams_render_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.render.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.render.position;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.render.width;
    
    }
  
  case CommandParams_rain_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.rain.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.rain.width;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.rain.drop_density;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.rain.drop_speed;
    
    }
  
  case CommandParams_gravity_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.gravity.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.gravity.launch_speed;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.gravity.ball_rate;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.gravity.width;
    
    }
  
  case CommandParams_kaleidoscope_tag:
    switch (field_index)
    {
    
    }
  
  case CommandParams_mapped_shape_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.mapped_shape.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.mapped_shape.x;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.mapped_shape.y;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.mapped_shape.size;
    
    case MIN_INDEX + 4:
      return &cmd->type_params.mapped_shape.fade_distance;
    
    }
  
  case CommandParams_mapped_slope_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.mapped_slope.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.mapped_slope.x1;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.mapped_slope.y1;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.mapped_slope.x2;
    
    case MIN_INDEX + 4:
      return &cmd->type_params.mapped_slope.y2;
    
    case MIN_INDEX + 5:
      return &cmd->type_params.mapped_slope.fade_distance;
    
    }
  
  case CommandParams_mapped_triangle_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.mapped_triangle.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.mapped_triangle.x1;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.mapped_triangle.y1;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.mapped_triangle.x2;
    
    case MIN_INDEX + 4:
      return &cmd->type_params.mapped_triangle.y2;
    
    case MIN_INDEX + 5:
      return &cmd->type_params.mapped_triangle.x3;
    
    case MIN_INDEX + 6:
      return &cmd->type_params.mapped_triangle.y3;
    
    }
  
  case CommandParams_mapped_particles_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.mapped_particles.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.mapped_particles.x;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.mapped_particles.y;
    
    case MIN_INDEX + 3:
      return &cmd->type_params.mapped_particles.size;
    
    case MIN_INDEX + 4:
      return &cmd->type_params.mapped_particles.fade_distance;
    
    }
  
  case CommandParams_mapped_ping_pong_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.mapped_ping_pong.color;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.mapped_ping_pong.direction;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.mapped_ping_pong.fade_distance;
    
    }
  
  case CommandParams_twang_tag:
    switch (field_index)
    {
    
    }
  
  case CommandParams_fairy_light_tag:
    switch (field_index)
    {
    
    case MIN_INDEX + 0:
      return &cmd->type_params.fairy_light.pattern;
    
    case MIN_INDEX + 1:
      return &cmd->type_params.fairy_light.frequency;
    
    case MIN_INDEX + 2:
      return &cmd->type_params.fairy_light.brightness;
    
    }
  
  }
}
