# default location for binaries installed by platformio IDE in vscode
pio_binary=~/.platformio/penv/bin/platformio

shell:
	nix-shell

generate:
	python3 nanopb/nanopb_generator.py --cpp-descriptors -S .cpp -D lib/Generated/ command_schemas.proto 
	elixir generate_field_selectors.exs
	
monitor:
	screen /dev/ttyUSB0 921600

wand:
	$(pio_binary) run --target upload --environment wand_wifi

xhain: tr33 ranken scubar wolken trommel

wolken: wolke1 wolke2 wolke3 wolke4 wolke5 wolke6 wolke8 wolke9 wolke10 wolke11 wolke12

wolke1:
	$(pio_binary) run --target upload -e wolke1

wolke2:
	$(pio_binary) run --target upload -e wolke2

wolke3:
	$(pio_binary) run --target upload -e wolke3

wolke4:
	$(pio_binary) run --target upload -e wolke4

wolke5:
	$(pio_binary) run --target upload -e wolke5

wolke6:
	$(pio_binary) run --target upload -e wolke6

wolke7:
	$(pio_binary) run --target upload -e wolke7

wolke8:
	$(pio_binary) run --target upload -e wolke8

wolke9:
	$(pio_binary) run --target upload -e wolke9

wolke10:
	$(pio_binary) run --target upload -e wolke10

wolke11:
	$(pio_binary) run --target upload -e wolke11

wolke12:
	$(pio_binary) run --target upload -e wolke12

wolke13:
	$(pio_binary) run --target upload -e wolke13

tr33:
	$(pio_binary) run --target upload -e tr33

trommel:
	$(pio_binary) run --target upload -e trommel

ranken:
	$(pio_binary) run --target upload -e ranken

scubar:
	$(pio_binary) run --target upload -e scubar