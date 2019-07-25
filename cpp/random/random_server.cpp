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
    Status Generate(ServerContext* context, RandomRequest* request, RandomResponse* response) override
    {
        int len = request->size();
        cout << "random len is:"<< len <<endl;
        unsigned char data[128]="0123456789ABCDEF0123456789ABCDEF";
        response->set_random(data,len);
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    RandomServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc,char **argv)
{
    RunServer();
    return 0;
}



