#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "msg.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class MsgServiceImpl final : public MsgService::Service
{

    Status Search(ServerContext *context, const MsgRequest *request, MsgResponse *response)
    {
        MsgRequest::RqInfoCase t_case = request->rq_info_case();
        char name[256] = {0};
        char id[20] = {0};
        std::string address = "wuhan";
        std::string phonenumber = "18507148681";
        switch (t_case)
        {
        case MsgRequest::kName: //has_name
            memcpy(name, request->name().data(), request->name().size());
            printf("name::%s .\n", name);
            response -> set_address(address);
            break;
        case MsgRequest::kId: //has_id
            memcpy(id, request->id().data(), request->name().size());
            printf("id::%s .\n", id);
            response -> set_phonenumber(phonenumber);
            break;
        case MsgRequest::RS_INFO_NOT_SET: //has not name or id
            printf("not has any name or id .\n");
            break;
        }
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
