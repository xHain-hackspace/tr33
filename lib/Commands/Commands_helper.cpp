#include <Commands.h>

float Commands::ping_pong(uint8_t period_100ms)
{
  int period = period_100ms * 100;
  int rem = millis() % period;

  if (rem <= period / 2)
  {
    return rem / (period / 2);
  }
  else
  {
    return 1 - ((rem - period / 2) / (period / 2));
  }

  //  period = period * 1000
  //   offset = offset * 1000

  //   case rem(System.os_time(:millisecond) + offset, period) do
  //     rem when rem <= period / 2 -> rem / (period / 2)
  //     rem -> 1 - (rem - period / 2) / (period / 2)
  //   end
}
