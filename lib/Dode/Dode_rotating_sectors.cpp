#include <Dode.h>
#include <Commands.h>
#include <math.h>

#define MAX_NR_OF_SECTORS 255

void Dode::rotating_sectors(uint8_t *data)
{
  uint8_t color_index = data[0];
  uint8_t brightness = data[1];
  uint8_t input_period = data[2]; //*100ms
  uint8_t input_offset = data[3]; //255 =360 degrees
  uint8_t nr_of_sectors = data[4];
  uint8_t input_width = data[5]; //255 =360 degrees

  float offset = input_offset / 255.0 * 2.0 * PI; //radians
  float width = input_width / 255.0 * 2.0 * PI;   //radians
  float rotation_period = input_period * 100.0;   //ms
  CRGB sector_color = ColorFromPalette(currentPalette, color_index, brightness);
  float sectors_offset = 2 * PI / nr_of_sectors;

  float start_angles[MAX_NR_OF_SECTORS];
  float end_angles[MAX_NR_OF_SECTORS];

  float segment_start_angle;
  float segment_end_angle;

  //calculate current starting position of sectors
  float start_angle = (millis() / rotation_period) * 2 * PI;
  start_angle = fmod(start_angle, 2 * PI); //account for angle overflow at 2*pi

  //populate array of start and end angles of individual sectors
  for (int curr_segment = 0; curr_segment < nr_of_sectors; curr_segment++)
  {
    start_angles[curr_segment] = fmod(start_angle + offset + curr_segment * sectors_offset, 2 * PI);
    end_angles[curr_segment] = fmod(start_angles[curr_segment] + width, 2 * PI);
  }
  for (int current_led = 0; current_led < NR_TOTAL_LEDS; current_led++)
  { //step through all LEDs
    for (int curr_segment = 0; curr_segment < nr_of_sectors; curr_segment++)
    { //check for each segment if led is in it
      segment_start_angle = start_angles[curr_segment];
      segment_end_angle = end_angles[curr_segment];
      if (segment_end_angle > segment_start_angle)
      { //normal case, segment does not include zero/origin
        if (coordinates[current_led].phi >= segment_start_angle && coordinates[current_led].phi <= segment_end_angle)
        {
          set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, sector_color);
          break;
        }
      }
      else
      { //special case with origin in segment, end is smaller than start
        if ((coordinates[current_led].phi >= segment_start_angle && coordinates[current_led].phi <= 2 * PI) || (coordinates[current_led].phi <= segment_end_angle && coordinates[current_led].phi > 0))
        {
          set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, sector_color);
          break;
        }
      }
      // float brightness = 1.0 - fabs(led_position - position) / (width / 2.0);

      // if (brightness > 0)
      // {
      //   brightness = Commands::ease_in_out_cubic(brightness * 1.0);
      //   fade_led(coordinates[curr_segment].i_edge, coordinates[curr_segment].i_led, color, brightness);
      // }
    }
  }
}
