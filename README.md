## Flash Firmware to ESP32

You need the `platformio` binary installed as the main dependencies. The required libraries for the project should be fetched automatically. You can either install platformio as vscode extension or just the cli tools. See [here](https://platformio.org/install/cli) for more details.

The `Makefile` contains all led structures that are supported out of the box. Each led structure has a separate make target that will flash the corresponding firmware. For example 

```
make ranken
```

will flash the `Ranken` led structure. It's a simple structure that consists of multiple led strips. Details can be configured in the file `lib/Ranken/Ranken_config.h`.

## WiFi Secrets

You need to create a `src/Secrets_xyz.h` file that contains the wifi secrets. The ESP32 will automatically connect to this WiFi. For the actual filename you can check 'DSECRETS_FILE=' in platformio.ini for your target or compile without the file and get the correct name from the error message. The file should look as follows:

```
const char *ssid = ""; // String
const char *password = ""; // string
const char *control_host = ""; // String [IPv4 or Domain]. Optional, is used to get commands from a tr33_control frontend on startup. Makes syncing more reliable in lossy networks.
```
If the board is trying to connect some LEDs will blink yellow/red. If the yellow/red blinking continues, no WiFi connection can be made. When the connection is sucessful the LEDs will turn of and do the normal effects.
### Known Problems with WiFi Secrets
If you change contents of the secrets file contents, it might sometimes get cached when using vscode and platformio, and will not be updated on the next build. The symptom of this is that e.g. the board still tries to connect with the old WiFi setting (which you can see when monitoring the debug output via USB-UART) and the connection will fail. As a workaround you can recompile with a fake filename for the secrets to force rebuild and then change it back.
I.e. in platformio.ini, for the target you are using, change:
```
Change in platformio.ini:
-DSECRETS_FILE='"Secrets_keller.h"' -> -DSECRETS_FILE='"Secrets_keller_1.h"'
(Do upload, fails with error)
Change back:
-DSECRETS_FILE='"Secrets_keller_1.h"' -> -DSECRETS_FILE='"Secrets_keller.h"'
(Do upload again, code should now be uploaded with the new secrets)
```

## Inital Commands

The `init` function in `lib/LedStructure/LedStructure.cpp` define the initial commands that will be rendered after the firmwared boots. 

## Sending Commands via Network

The commands rendered on the led structure can be changed by sending protobuf commands via UDP to port 1337. The protobuf schema is in `command_schemas.proto`.

## Connecting to the tr33_control frontend

There is a web frontend called [tr33_control](https://github.com/xHain-hackspace/tr33_control) that can send commands to multiple led structures in parallel. The README contains setup instructions.

## Mapping

Some commands support mapping the leds to xy positions. The mapping for a led structure can be generated with [tr33-pixel-mapping](https://github.com/xHain-hackspace/tr33-pixel-mapping). Update the mapping file with the generated content. For example the mapping for `Ranken` is in `lib/Ranken/Ranken_mapping.cpp`. Also the min/max values in `Rankine_config.h` need to be updated.



