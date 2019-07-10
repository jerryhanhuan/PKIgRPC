#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "search.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;



// final ,C++11标准中，引入的关键字，实现 final 类。一个类不能被别的类继承，这种类被称为 final 类
// override 明确地表示一个函数是对基类中一个虚函数的重载
class SearchRequestImpl final : public SearchService::Service
{
    Status SearchService(ServerContext *context, const SearchRequest *request, SearchResponse *response) override{
        std::string prefix("Hello ");
        response->set_response(prefix + request->request());
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    SearchRequestImpl service;

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
