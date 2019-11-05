

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

#include <grpcpp/grpcpp.h>
#include "wrapkeys.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class WrapkeyImpl final : public WrapKey::Service
{

public:
    Status Wrap(ServerContext *context, const Cipher *request, Cipher *response) override
    {
        string data;
        cout << "transformation::" << request->transformation() << endl;
        for(auto &msg:request->messages())
        {
            data += msg;
        }
        cout << "msg:" << data << endl;
        for(auto key:request->wrappingkeys())
        {
            cout << "key::" << key.secret_key() << endl;
        }
        return Status::OK;
    }
};



void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    WrapkeyImpl service;

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
