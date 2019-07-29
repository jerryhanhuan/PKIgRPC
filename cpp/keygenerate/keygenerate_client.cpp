#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "keygenerate.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class keygenerateClient
{

public:
    keygenerateClient(std::shared_ptr<Channel> channel)
        : stub_(KeyGenerateService::NewStub(channel)) {}

    int genkey(std::string &keyType, int keysize, std::string &curve)
    {
        int i = 0;
        KeyGenerateRequest request;
        KeyGenerateResponse response;
        request.set_transformation(keyType);
        request.set_key_size(keysize);
        request.set_curve(curve);
        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        //实际 RPC 调用
        Status status = stub_->Generate(&context, request, &response);
        if (status.ok())
        {

            KeyCase key_s = response.key_case();
            if (key_s == kSecretKey)
            {
                unsigned char key[32] = {0};
                memcpy(key, response.secret_key().data(), response.secret_key().size());
                
                printf("secret key::");
                for (i = 0; i < response.secret_key().size(); i++)
                {
                    printf("%02X", key[i]);
                }
                printf("\n");
            }
            else if (key_s == kKeyPair)
            {

                KeyGenerateResponse_KeyPair keypair;
                keypair = response.key_pair();
                unsigned char vk[32] = {0};
                unsigned char pk[64] = {0};
                memcpy(pk, keypair.public_key().data(), keypair.public_key().size());
                memcpy(vk, keypair.private_key().data(), keypair.private_key().size());
                printf("vk::");
                for (i = 0; i < keypair.private_key().size(); i++)
                {
                    printf("%02X", vk[i]);
                }
                printf("\n");

                printf("pk::");
                for (i = 0; i < keypair.public_key().size(); i++)
                {
                    printf("%02X", pk[i]);
                }
                printf("\n");
            }
            return 0;
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<KeyGenerateService::Stub> stub_;
};


int main(int argc, char **argv)
{

    keygenerateClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
	
	std::string keytype = "SM4";
	int keylen = 16;
	std::string curve = "SYM";
    //sm4
    cli.genkey(keytype,keylen,curve);
	keytype = "SM2";
	keylen = 256;
	curve = "EC";
    cli.genkey(keytype,keylen,curve);

    return 0;
}
