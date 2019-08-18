#include <Dode.h>
#include <Commands.h>

#define MAX_SPARKS 64

struct Spark
{
  int8_t edge;
};

Spark sparks[MAX_SPARKS];
uint8_t active_sparks = 0;
float relative_position = 0.0;
long last_update = millis();

void Dode::fireworks(char *data)
{
  uint8_t color_index = data[0];
  float brightness = data[1];
  uint8_t rate = data[2];
  float width = float(data[3]);

  last_update = millis();
}
