#include <Arduino.h>
#include <FastLED.h>

#define COMMAND_DATA_SIZE 8
#define COMMAND_BUFFER_SIZE 16

#define MODE_COMMANDS 0
#define MODE_STREAM 1

// colors
#define HUE_RED 0
#define HUE_YELLOW 42
#define HUE_GREEN 85
#define HUE_BLUE 171
#define HUE_PINK 213
#define COLOR_BLACK CHSV(0, 0, 0)
#define COLOR_WHITE CRGB(255, 255, 255)

struct Command
{
  uint8_t index;
  uint8_t type;
  char data[COMMAND_DATA_SIZE];
};

extern Command command_buffer[COMMAND_BUFFER_SIZE];
extern CRGBPalette256 currentPalette;
extern uint8_t currentMode;

class Commands
{
public:
  void init();
  void process(char *command);
  void run();
  void update_settings(char *data);
};