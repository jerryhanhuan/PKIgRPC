#include <iostream>
#include <memory>
#include <string>



using std::cout;
using std::endl;


#include <grpcpp/grpcpp.h>

#include "random.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class RandomServiceImpl final : public  RandomService::Service
{
    Status Generate(ServerContext* context, const RandomRequest* request, RandomResponse* response) override{
        int len = request->size();
        cout << "random len is:"<< len <<endl;
        unsigned char data[128]={0x12,0x34,0x00,0x56,0x78,0xAB,0xCD,0xEF,0xEF,0xCD,0xAB,0x78,0x56,0x00,0x34,0x12};
        response->set_random(data,len);
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    RandomServiceImpl service;

    ServerBuilder builder;
    //builder.SetOption(std::unique_ptr<grpc::ServerBuilderOption>(new NoReusePortOption));
     // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc,char **argv)
{
    RunServer();
    return 0;
}



