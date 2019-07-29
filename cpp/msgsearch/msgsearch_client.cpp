
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "msg.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class msgSearchClient
{

public:
    msgSearchClient(std::shared_ptr<Channel> channel)
        : stub_(MsgService::NewStub(channel)) {}

    int msgSearch(std::string name, std::string id)
    {
        MsgRequest request;
        MsgResponse response;
        char address[256] = {0};
        char phonenum[32] = {0};
        if (name.size())
        {
            request.set_name(name);
        }
        else
        {
            request.set_id(id);
        }
        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        //实际 RPC 调用
        Status status = stub_->Search(&context, &request, &response);
        if (status.ok())
        {
            MsgResponse::RsInfoCase t_case = response.rs_info_case();
            switch (t_case)
            {
            case MsgResponse::kAddress: //has_address
                memcpy(address, response.address().data(), response.address().size());
                printf("address::%s .\n", address);
                break;
            case MsgResponse::kPhonenumber: //has_phonenumber
                memcpy(phonenum, response.phonenumber().data(), response.phonenumber().size());
                printf("phonenum::%s .\n", phonenum);
                break;
            case MsgResponse::RS_INFO_NOT_SET: //has not name or id
                printf("not has any address or phonenumber .\n");
                break;
            }
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<MsgService::Stub> stub_;
};

int main()
{

    msgSearchClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string name = "yexb";
    std::string id = "1234567";

    cli.msgSearch(name,"");
    cli.msgSearch("",id);

    return 0;
}