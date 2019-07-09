all:

	# helloworld
	# generate helloworld.grpc.pb.h helloworld.grpc.pb.cc
	protoc -I ../protos --grpc_out=helloworld --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/helloworld.proto
	# generate helloworld.pb.h helloworld.pb.cc
	protoc -I ../protos --cpp_out=helloworld ../protos/helloworld.proto
	make -C helloworld

.PHONY:clean

clean:
	make clean -C helloworld
