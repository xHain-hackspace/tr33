#include <Dode.h>
#include <Commands.h>

long last_debug_update = 0;

void Dode::debug(char *data)
{
  float edge = 0.25*3.1415*(fabs(Commands::ping_pong_linear(100,0))*2-1);//1.4 * (fabs(Commands::ping_pong_linear(100,0))*2-1);
  for (int current_led=0; current_led < NR_TOTAL_LEDS; current_led++){
    if (coordinates[current_led].phi >= edge){
      set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, CHSV(HUE_GREEN, 0, 255)); 
    }else{
      set_led(coordinates[current_led].i_edge, coordinates[current_led].i_led, CHSV(HUE_RED, 255, 0)); 
    }
  }
}
