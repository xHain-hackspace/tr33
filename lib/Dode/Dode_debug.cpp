#include <Dode.h>
#include <Commands.h>
#include <math.h>

#define MAX_NR_OF_SECTORS 400

long last_sectors_update = 0;
float last_start_angle = 0;

void Dode::debug(char *data)
{
  float width = 30/360.0*2.0*PI; //radians
  float rotation_period =15000;//ms
  CHSV sector_color = CHSV(HUE_GREEN,255,126);
  int nr_of_sectors =3;
  float sectors_offset = 2*PI/nr_of_sectors;

  float start_angles[MAX_NR_OF_SECTORS];
  float end_angles[MAX_NR_OF_SECTORS];

  float segment_start_angle;
  float segment_end_angle;

  //calculate elapsed time since last update
  long elapsed_time = millis()- last_sectors_update;
  last_sectors_update = millis();

  //calculate current starting position of sectors
  float start_angle = (elapsed_time/rotation_period)*2*PI+last_start_angle;
  start_angle=fmod(start_angle,2*PI);//account for angle overflow at 2*pi
  last_start_angle = start_angle;

  //populate array of start and end angles of individual sectors
  for (int curr_segment=0;curr_segment< nr_of_sectors;curr_segment++){
    start_angles[curr_segment]=fmod(start_angle+curr_segment*sectors_offset,2*PI) ;
    end_angles[curr_segment] =fmod(start_angles[curr_segment] + width, 2*PI) ;
  }
    
  for (int current_led=0; current_led < NR_TOTAL_LEDS; current_led++){//step through all LEDs
    for (int curr_segment=0;curr_segment< nr_of_sectors;curr_segment++){//check for each segment if led is in it
      segment_start_angle =start_angles[curr_segment];
      segment_end_angle = end_angles[curr_segment];
      if (segment_end_angle > segment_start_angle){//normal case, segment does not include zero/origin
        if (coordinates[current_led].phi >= segment_start_angle && coordinates[current_led].phi <= segment_end_angle){
          set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, sector_color); 
        }
      }else{//special case with origin in segment, end is smaller than start
        if ((coordinates[current_led].phi >= segment_start_angle && coordinates[current_led].phi <= 2*PI) || (coordinates[current_led].phi <= segment_end_angle && coordinates[current_led].phi > 0)){
          set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, sector_color); 
        }
      }
    }
  }
}
