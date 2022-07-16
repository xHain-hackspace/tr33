## Flash Firmware to ESP32

You need the `platformio` binary installed as the main dependencies. The required libraries for the project should be fetched automatically. You can either install platformio as vscode extension or just the cli tools. See [here](https://platformio.org/install/cli) for more details.

The `Makefile` contains all led structures that are supported out of the box. Each has a separate make target that will flash the corresponding firmware. For example 

```
make ranken
```

will flash for a simple led structure that consists of multiple led strips. Details can be configured in the file `lib/Ranken/Ranken_config.h`.

## WiFi Secrets

You need to create a `src/Secrets.h` file that contains the wifi secrets. The ESP32 will automatically connect to this WiFi. The file should look as follows:

```
const char *ssid = ""; // String
const char *password = ""; // string
const char *control_host = ""; // String [IPv4 or Domain]. Optional, is used to get commands from a tr33_control frontend on startup. Makes syncing more reliable in lossy networks.
```

## Inital Commands

The `init` function in `lib/LedStructure/LedStructure.cpp` define the initial commands that will be rendered after the firmwared boots. 

## Sending Commands via Network

The commands rendered on the led structure can be changed by sending protobuf commands via UDP to port 1337. The protobuf schema is in `command_schemas.proto`.

## Connecting to the tr33_control frontend

There is a web frontend called [tr33_control](https://github.com/xHain-hackspace/tr33_control) that can send commands to multiple led structures in parallel. The README contains setup instructions.

## Mapping

Some commands support mapping the leds to xy positions. The mapping for a led structure can be generated with [tr33-pixel-mapping](https://github.com/xHain-hackspace/tr33-pixel-mapping). Update the mapping file with the generated content. For example the mapping for `Ranken` is in `lib/Ranken/Ranken_mapping.cpp`. Also the min/max values in `Rankine_config.h` need to be updated.



