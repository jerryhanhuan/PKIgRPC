#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <stdio.h>

using std::cout;
using std::endl;

#include <grpcpp/grpcpp.h>

#include "pbkdf.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;

class PBKDFServiceImpl final : public PBKDFService::Service
{
    Status Pbkdf(ServerContext *context, const PBKDFRequest *request, PBKDFResponse *response) override
    {
        std::string pbkdf_password = "";
        int keysize = 0;
        std::string hash = "HMAC-SHA1"; //默认是 HMAC-SHA1
        int iterations = 10000;
        int ret = 0;

        pbkdf_password = request->password();
        keysize = request->key_size();
        hash = request->pseudo_random_function();
        iterations = request->iterations();

        printf("in PBKDFServiceImpl::Pbkdf::pbkdf_password[%s] .\n", pbkdf_password.data());
        printf("in PBKDFServiceImpl::Pbkdf::keysize[%d] .\n", keysize);
        printf("in PBKDFServiceImpl::Pbkdf::hash[%s] .\n", hash);
        printf("in PBKDFServiceImpl::Pbkdf::iterations[%d] .\n", iterations);

        for(auto msg:request->salts())
        {
            cout << msg <<endl;
        }

        std::string key = "1234567890112345678";
        response->set_secret_key(key);
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    PBKDFServiceImpl service;

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
