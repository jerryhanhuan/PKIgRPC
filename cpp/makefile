all:

	# helloworld
	# generate helloworld.grpc.pb.h helloworld.grpc.pb.cc
	protoc -I ../protos --grpc_out=helloworld --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/helloworld.proto
	# generate helloworld.pb.h helloworld.pb.cc
	protoc -I ../protos --cpp_out=helloworld ../protos/helloworld.proto
	make -C helloworld

	#search
	protoc -I ../protos --cpp_out=search ../protos/search.proto
	protoc -I ../protos --grpc_out=search --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/search.proto
	make -C search

	#random
	protoc -I ../protos --cpp_out=random ../protos/random.proto
	protoc -I ../protos --grpc_out=random --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/random.proto
	make -C random

	#digest
	protoc -I ../protos --cpp_out=digest ../protos/digest.proto
	protoc -I ../protos --grpc_out=digest --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/digest.proto
	make -C digest


	#stream
	protoc -I ../protos --cpp_out=stream ../protos/stream.proto
	protoc -I ../protos --grpc_out=stream --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/stream.proto
	make -C stream

	# keygenerate
	protoc -I ../protos --cpp_out=keygenerate ../protos/keygenerate.proto
	protoc -I ../protos --grpc_out=keygenerate --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/keygenerate.proto
	make -C keygenerate

	# msgsearch
	protoc -I ../protos --cpp_out=msgsearch ../protos/msg.proto
	protoc -I ../protos --grpc_out=msgsearch --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/msg.proto
	make -C msgsearch


	# pbkdf
	protoc -I ../protos --cpp_out=pbkdf  ../protos/pbkdf.proto
	protoc -I ../protos --grpc_out=pbkdf --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/pbkdf.proto
	make -C pbkdf

	# transferfile
	protoc -I ../protos --cpp_out=transferfile ../protos/transferfile.proto
	protoc -I ../protos --grpc_out=transferfile --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../protos/transferfile.proto
	make -C transferfile


.PHONY:clean

clean:
	make clean -C helloworld
	make clean -C search
	make clean -C random
	make clean -C digest
	make clean -C stream
	make clean -C keygenerate
	make clean -C msgsearch
	make clean -C pbkdf
