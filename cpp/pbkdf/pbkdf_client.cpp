#include <iostream>
#include <memory>
#include <string>

#include <stdio.h>

#include <grpcpp/grpcpp.h>

#include "pbkdf.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using std::cout;
using std::endl;

class PBKDFClient
{

public:
    PBKDFClient(std::shared_ptr<Channel> channel)
        : stub_(PBKDFService::NewStub(channel)) {}

    int pbkdf(const std::string &password, int key_size, const std::string &hash, unsigned char *salts, int saltlen, int iterations, unsigned char *key)
    {
        PBKDFRequest request;
        PBKDFResponse response;
        request.set_password(password);
        request.set_key_size(key_size);
        // hash 是可选项
        if (hash.length())
        {
            request.set_pseudo_random_function(hash);
        }
        // iteration
        //request.set_iterations(iterations);

        const int len = 8;
        int rlen = 0;
        int left = 0;

        while (rlen < saltlen)
        {
            left = saltlen - rlen;
            if (left >= len)
            {
                request.add_salts(salts + rlen, len);
                rlen += len;
            }
            else
            {
                request.add_salts(salts + rlen, left);
                rlen += left;
            }
        }

        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        //实际 RPC 调用
        Status status = stub_->Pbkdf(&context, request, &response);
        if (status.ok())
        {
            memcpy(key, response.secret_key().data(), response.secret_key().size());
            return response.secret_key().size();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<PBKDFService::Stub> stub_;
};

int main(int argc, char **argv)
{

    PBKDFClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string password = "123456";
    int keysize = 16;
    const char *salts = "0123456789ABCDEFFEDCBA987654321000000000000000000000000000000000FEDCBA98765432100123456789ABCDEF";
    unsigned char key[1024] = {0};
    if (cli.pbkdf(password, 16, "", (unsigned char*)salts, strlen((char *)salts), 100, key) < 0)
    {
        std::cout << "Random failed !" << endl;
    }
    else
    {

        printf("%s .\n", key);
    }
    return 0;
}
