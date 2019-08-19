#include <Dode.h>
#include <Commands.h>

long last_debug_update = 0;

void Dode::debug(char *data)
{
  float pos_neg_pingpong = (fabs(Commands::ping_pong_linear(1500,0))*2-1);//from -1 to +1
  float edge = pos_neg_pingpong*3.1415;
  //float edge = 1.4 * (fabs(Commands::ping_pong_linear(100,0))*2-1);
  for (int current_led=0; current_led < NR_TOTAL_LEDS; current_led++){
    if (coordinates[current_led].phi >= edge){
      set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, CHSV(HUE_GREEN, 255, 0)); 
    }else{
      set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, CHSV(HUE_RED, 0, 255)); 
    }
  }
}
