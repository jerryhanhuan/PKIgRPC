all:server\
	client\
	final

.PHONY:all

CC = g++
CXXFLAG = -std=c++11
INC = -I ./  `pkg-config --cflags protobuf grpc`
DEFINES =  -g $(INC) $(CXXFLAG)
LIBS = -lgrpc++ -lprotobuf -lpthread -lgrpc++_reflection
#LIBS = `pkg-config  --libs protobuf grpc++ grpc`


grpc_obj = keygenerate.pb.o keygenerate.grpc.pb.o 
server_obj = keygenerate_server.o
client_obj = keygenerate_client.o

%.o:%.cc
	$(CC) $(DEFINES) -c $< 

%.o:%.cpp
	$(CC) $(DEFINES) -c $<

client:$(client_obj) $(grpc_obj) $(LIBS)
	$(CC) -o $@ $^ 

server:$(server_obj) $(grpc_obj) $(LIBS)
	$(CC) -o $@ $^

final:
	rm -rf *.o

.PHONY:clean

clean:
	rm -rf *.o
	rm -rf server
	rm -rf client



