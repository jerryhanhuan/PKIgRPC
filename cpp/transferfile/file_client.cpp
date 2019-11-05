#include <iostream>
#include <memory>
#include <string>
#include <fstream>

#include <sys/time.h>
#include <string.h>

#include <grpcpp/grpcpp.h>
#include "transferfile.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientWriter;
using grpc::Status;

using namespace std;

#define CHUNK_SIZE 1024 * 1024

class TransferFileClient
{

public:
    TransferFileClient(std::shared_ptr<Channel> channel) : stub_(TransferFile::NewStub(channel)){};
    long Upload(const char* filename);

private:
    // 注意，这个不要忘
    std::unique_ptr<TransferFile::Stub> stub_;
};

long TransferFileClient::Upload(const char *filename)
{

    Chunk chunk;
    Reply response;
    char data[CHUNK_SIZE];
    long len = 0;
    struct timeval start, end;
    std::ifstream infile;
    ClientContext context;

    gettimeofday(&start, NULL);
    infile.open(filename, std::ifstream::in | std::ifstream::binary);
    std::unique_ptr<ClientWriter<Chunk>> writer(stub_->Upload(&context, &response));

    while (!infile.eof())
    {
        infile.read(data, CHUNK_SIZE);
        // streamsize gcount();
        // 函数gcount()被用于输入流，并返回上一次输入操作被读入的字符的数目
        chunk.set_buffer(data, infile.gcount());
        if (!writer->Write(chunk))
        {
            // Broken stream.
            break;
        }
        len += infile.gcount();
    }
    infile.close();
    //WritesDone() 通知gRPC 我们已经完成输入
    writer->WritesDone();
    Status status = writer->Finish();
    if (status.ok())
    {
        gettimeofday(&end, NULL);
        std::cout << "spend time(s)::" << (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000 << std::endl;
        cout << "file size ::" << std::fixed << response.length() << endl;
    }
    else
    {
        std::cout << "TransferFile rpc failed." << std::endl;
    }
	return 0;
}



int main(int argc, char **argv)
{

    string ip;
    int port;
    cout << "server ip and port::" <<endl;
    cin >> ip >> port;

    

    string filename;
    cout << "请输入文件名::" << endl;
    cin >> filename;

    char address[256]={0};
    sprintf(address, "%s:%d", ip.c_str(), port);


    TransferFileClient cli(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    cli.Upload(filename.c_str());

    return 0;
}
