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
        std::unique_ptr<ClientReader<StreamResData>> cli_reader(
            stub_->GetStream(&context, requset));

        while (cli_reader->Read(&response))
        {
            std::cout << "res::" << response.res_data() << std::endl;
        }
        Status status = cli_reader->Finish();
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
        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        StreamReqData requset;
        StreamResData response;
        std::unique_ptr<ClientWriter<StreamReqData>> cli_writer(
            stub_->PutStream(&context, &response));
        char msg[3][256] = {"this is the first msg", "this is the second msg", "this is the third msg"};
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            requset.set_req_data(msg[i]);
            if (!cli_writer->Write(requset))
            {
                // Broken stream.
                break;
            }
        }
        //WritesDone() 通知gRPC 我们已经完成输入
        cli_writer->WritesDone();
        //Finish() 完成调用同时拿到 RPC 的状态
        Status status = cli_writer->Finish() if (status.ok())
        {
            std::string result = response.res_data();
            std::cout << "res::" << result << std::endl;
            return 0;
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
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

    std::cout << " ========================= server stream =========================" <<std::endl;
    if (cli.getStream() < 0)
    {
        std::cout << "getStream failed !" << endl;
    }
    std::cout << " ========================= client stream =========================" <<std::endl;
    if (cli.putStream() < 0)
    {
        std::cout << "putStream failed !"<<endl;
    }
    return 0;
}