install:
	pip install protobuf

# This is need after changes to command_schemas.proto. Currently requires elixir and protobuf python packages, which is not ideal and will be fixed at some point
generate:
	python3 nanopb/nanopb_generator.py --cpp-descriptors -S .cpp -D lib/Generated/ command_schemas.proto 
	elixir generate_field_selectors.exs
	
monitor:
	screen /dev/ttyUSB0 921600