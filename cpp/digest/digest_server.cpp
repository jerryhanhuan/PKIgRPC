#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;

#include <grpcpp/grpcpp.h>

#include "digest.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;

class DigestServiceImpl final : public DigestService::Service
{
    Status Digest(ServerContext *context, ServerReader<::DigestRequest> *reader, ::DigestResponse *response) override
    {
        DigestRequest request;
        char digest_alg[32] = {0};
        unsigned char data[8192] = {0};
        int count = 0;
        while (reader->Read(&request))
        {
            if (count == 0)
            {
                strncpy(digest_alg, request.transformation().data(), request.transformation().size());
                cout << "digest_alg:" << digest_alg << endl;
            }
            cout << "size:"<<request.messages().size()<< "msg:"<<request.messages().data()<<endl;
            memset(data, 0, sizeof(data));
            memcpy(data, request.messages().data(), request.messages().size());
            count++;
        }
        unsigned char digest[32] = {0x12, 0x34, 0x00, 0x56, 0x78, 0xAB, 0xCD, 0xEF, 0xEF, 0xCD, 0xAB, 0x78, 0x56, 0x00, 0x34, 0x12};
        response->set_digest((char*)digest);
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    DigestServiceImpl service;

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
