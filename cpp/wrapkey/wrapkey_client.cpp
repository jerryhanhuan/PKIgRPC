
#include <iostream>
#include <memory>
#include <string>
#include <vector>


#include <grpcpp/grpcpp.h>
#include "wrapkeys.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


using std::cout;
using std::endl;
using std::string;
using std::vector;


class WrapkeyClient
{

public:
    WrapkeyClient(std::shared_ptr<Channel> channel)
        : stub_(WrapKey::NewStub(channel)) {}

    int Wrapkey()
    {
        int ret = 0;
        Cipher request;
        Cipher response;
        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        string transformation = "DES/ECB";
        request.set_transformation(transformation);

        vector<string> messageV;
        messageV.push_back("11111111");
        messageV.push_back("22222222");
        messageV.push_back("33333333");

        for (auto msg : messageV)
        {
            request.add_messages(msg);
        }

        vector<string> keyV;
        keyV.push_back("0123456789ABCDEFFEDCBA9876543210");
        keyV.push_back("00000000000000000000000000000000");
        keyV.push_back("FEDCBA98765432100123456789ABCDEF");

        for (auto key : keyV)
        {
            Key *t = request.add_wrappingkeys();
            t->set_secret_key(key);
        }
        //实际 RPC 调用
        Status status = stub_->Wrap(&context, request, &response);
        if (status.ok())
        {
            cout << "Wrap Key ok" << endl;
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
        return 0;
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<WrapKey::Stub> stub_;
};



int main(int argc, char **argv)
{

    WrapkeyClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    cli.Wrapkey();
    return 0;
}
