#include <Commands.h>
#include <Tr33.h>

Tr33 runner = Tr33();

Command command_buffer[COMMAND_BUFFER_SIZE];
CRGBPalette256 currentPalette = Rainbow_gp;
uint8_t currentMode = MODE_COMMANDS;

void Commands::init()
{
  FastLED.setCorrection(TypicalLEDStrip);
  runner.init();
};

void Commands::process(char *command_bin)
{
  Command command = *(Command *)command_bin;

  if (command.type > 100)
  {
    runner.process_event(&command);
  }
  else
  {
    if (command.index < COMMAND_BUFFER_SIZE)
    {
      command_buffer[command.index] = command;
    }
  }
};

void Commands::run()
{
  bool run_commands;

  switch (currentMode)
  {
  case MODE_STREAM:
    run_commands = false;
    break;
  default:
    run_commands = true;
    runner.all_off();
    break;
  }

  if (run_commands)
  {
    for (int i = 0; i < COMMAND_BUFFER_SIZE; i++)
    {
      runner.process_command(&command_buffer[i]);
    }
  }

  FastLED.show();
}