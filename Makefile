install:
	git clone git@github.com:nanopb/nanopb.git protobuf/

generate:
	python3 protobuf/generator/nanopb_generator.py --cpp-descriptors -S .cpp -D lib/Generated/ command_schemas.proto 
	elixir generate_field_selectors.exs
	