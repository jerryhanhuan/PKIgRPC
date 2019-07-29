#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "msg.grpc.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class MsgServiceImpl final:public MsgService::Service{

    Status Search(ServerContext* context, MsgRequest* request, MsgResponse* response)
    {
        enum  RqInfoCase req_case = request.rq_info_case();
        return Status::OK;
    }

};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    MsgServiceImpl service;

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


