#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "stream.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::Status;

using std::cout;
using std::endl;

class StreamClient
{

public:
    StreamClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    // 服务端推送流
    int getStream()
    {

        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        StreamReqData requset;
        StreamResData response;

        requset.set_req_data("client get server stream");
        std::unique_ptr<ClientReader<StreamResData>> cli_writer(
            stub_->GetStream(&context, requset));

        while (cli_writer->Read(&response))
        {
            cout << "res::" << response.res_data() << endl;
        }
        Status status = cli_writer->Finish();
        if (!status.ok())
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
        return 0;
    }

    // 客户端推送流程

    int putStream()
    {
    }

    // 客户端-服务端 双流式
    int allStream()
    {
        return 0;
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<Greeter::Stub> stub_;
};


int main(int argc, char **argv)
{

    StreamClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    if (cli.getStream() < 0)
    {
        std::cout << "hash failed !" << endl;
    }
    return 0;
}