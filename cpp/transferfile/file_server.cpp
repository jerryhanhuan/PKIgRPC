#include <iostream>
#include <memory>
#include <string>
#include <fstream>



#include <grpcpp/grpcpp.h>
#include "transferfile.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;


using std::cout;
using std::endl;

#define CHUNK_SIZE 1024 * 1024

class TransferFileImpl  final:public TransferFile::Service{
    Status Upload(ServerContext* context, ServerReader<Chunk>* reader,Reply* response) override
};




Status TransferFileImpl::Upload(ServerContext* context, ServerReader<Chunk>* reader,Reply* response)
{

    return Status::OK;
}


void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    TransferFileImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}


