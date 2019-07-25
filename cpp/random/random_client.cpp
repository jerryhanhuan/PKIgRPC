#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "random.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using std::cout;
using std::endl;

class RandomClient
{

public:
    RandomClient(std::shared_ptr<Channel> channel)
        : stub_(RandomService::NewStub(channel)) {}

    int Random(int size,unsigned char *randdata)
    {
        RandomRequest request;
        RandomResponse response;
        request.set_size(size);

        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        //实际 RPC 调用
        Status status = stub_->Generate(&context, request, &response);
        if(status.ok())
        {
                memcpy(randdata, response.random().c_str(),size);
                return size;
        }else{
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<RandomService::Stub> stub_;
};

int main(int argc, char **argv)
{

    RandomClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    int size = 16;
    unsigned char data[1024]={0};
    if(cli.Random(size,data) < 0)
    {
        std::cout << "Random failed !"<< endl;
    }else{
        int i = 0;
        for(i=0;i<size;i++)
        {
            printf("%02X",data[i])
        }
        print('\n');
    }
    return 0;
}
