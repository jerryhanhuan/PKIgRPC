#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "keygenerate.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class KeyGenerateServiceImpl final : public KeyGenerateService::Service {

    Status Generate(ServerContext* context, const KeyGenerateRequest* request, KeyGenerateResponse* response)
    {
            std::cout << "transformation::" << request ->transformation() << std::endl;
            std::cout << "keysize::" << request ->key_size() << std::endl;
            std::cout << "curve::" << request ->curve() << std::endl;

            unsigned char secret_key[16] = {0x00,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32};
            unsigned char pk[64] = {0x03,0x5D,0x36,0x5F,0x1B,0xE4,0xE2,0xB7,0x47,0x24,0x9F,0xC4,0x57,0xB2,0xE8,0x11,0xB5,0x70,0x39,0xBD,0x31,0xFB,0x8B,0x86,0xD4,0x06,0x9F,0x0D,0xA2,0x50,0x9A,0x92,0xE9,0x03,0x5B,0x13,0x1E,0xD1,0x72,0x67,0xDE,0x26,0xF2,0xED,0x72,0x31,0x28,0xF2,0xF5,0x25,0x79,0x48,0xD4,0x1B,0x4D,0x4F,0x5F,0x7F,0xD8,0x66,0x30,0x63,0xB4,0x41};
            unsigned char vk[32] = {0x28,0x8E,0xE3,0x3C,0xDD,0xD9,0xE5,0xE4,0x9E,0x4E,0xAB,0xC6,0xA5,0xD3,0x14,0xFF,0xB5,0xA0,0x17,0x06,0x03,0x51,0xE3,0x00,0xBD,0x62,0xE5,0x3F,0x1E,0xED,0x0A,0xF5};
            std::string sm4head = "SM4";
            std::string sm2head = "SM2";
            if(!sm4head.compare(request ->transformation()))
            {
                //SM4
                response -> set_secret_key(secret_key,16);

            }else if(!sm2head.compare(request -> transformation()))
            {
                //SM2
                response -> set_private_key(vk, 32);
                response -> set_public_key(pk, 64);
            }
            return Status::OK;
    }

};


void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    KeyGenerateServiceImpl service;

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