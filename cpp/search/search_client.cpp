#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "search.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


class SearchClient{

    public:
        SearchClient(std::shared_ptr<Channel> channel)
        : stub_(SearchService::NewStub(channel)){}
    
        std::string Search(const std::string& user){
            SearchRequest request;
            SearchResponse response;
            request.set_request(user);

            // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
            ClientContext context;

            //实际 RPC 调用
            Status status = stub_->SearchService(&context,request,&response);
            if(status.ok())
            {
                return response.response();
            }else{
                std::cout << status.error_code()<<": "<< status.error_message()<<std::endl;
            }
            return "RPC failed";

        }


};


int main(int argc,char **argv)
{

    SearchClient cli(grpc::CreateChannel("localhost:50051",grpc::InsecureChannelCredentials()));
    std::string user("yudf");
    std::string reply = cli.Search(user);
    std::cout <<"receive search result:"<<reply<<std::endl;
    return 0;
}
