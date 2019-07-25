#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "digest.grpc.pb.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using std::cout;
using std::endl;

class DigestClient
{
	public:
    DigestClient(std::shared_ptr<Channel> channel)
        : stub_(DigestService::NewStub(channel)) {}

    int hash(int hash_flag, unsigned char *data, int datalen, unsigned char *digest)
    {

        // clietn context 用来向服务器传递额外的信息，并调整某些 RPC 行为
        ClientContext context;
        DigestResponse response;
        DigestRequest request;
        char digest_alg[32] = {0};
        strcpy(digest_alg,"SHA1");
        std::unique_ptr<ClientWriter<DigestRequest>> cli_writer(
            stub_->Digest(&context, &response));
        int i = 0;
        char cmsg[3][256] = {"1111", "2222", "3333"};
        for (i = 0; i < 3; i++)
        {
            request.set_transformation(digest_alg);
            request.add_messages(cmsg[i]);
            if (!cli_writer->Write(request))
                break;
        }
        //WritesDone() 通知gRPC 我们已经完成输入
        cli_writer->WritesDone();
        //Finish() 完成调用同时拿到 RPC 的状态
        Status status = cli_writer->Finish();
        if (status.ok())
        {
            i = 0;
            memcpy(digest, response.digest().data(), response.digest().size());
            return 16;
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return -1;
        }
    }

private:
    // 注意，这个不要忘
    std::unique_ptr<DigestService::Stub> stub_;
};

int main(int argc, char **argv)
{

    DigestClient cli(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    int size = 16;
    unsigned char data[1024] = {0};
    if (cli.hash(0, NULL, 0, data) < 0)
    {
        std::cout << "hash failed !" << endl;
    }
    else
    {
        int i = 0;
        for (i = 0; i < 16; i++)
        {
            printf("%02X", data[i]);
        }
        printf("\n");
    }
    return 0;
}
