protobuf-install:
	git clone git@github.com:nanopb/nanopb.git protobuf/

protobuf-generate:
	python3 protobuf/generator/nanopb_generator.py --cpp-descriptors -S .cpp -D lib/Schemas/ command_schemas.proto 
	# protoc --include-imports --cpp_out=lib/Schemas/ command_schemas.proto 
	