#include <iostream>
#include <memory>
#include <string>
#include <fstream>



#include <grpcpp/grpcpp.h>
#include "transferfile.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;


using std::cout;
using std::endl;

#define CHUNK_SIZE 1024 * 1024

class TransferFileImpl  final:public TransferFile::Service{
    Status Upload(ServerContext* context, ServerReader<Chunk>* reader,Reply* response) override;
};




Status TransferFileImpl::Upload(ServerContext* context, ServerReader<Chunk>* reader,Reply* response)
{

    Chunk chunk;
    const char *des_file_name = "server_tmp";
    std::ofstream outfile;
    std::string data;

    // out 文件以输出方式打开(写文件)
    // trunc 如果文件存在，把文件长度设置为0
    // binary 以二进制方式读写文件
    outfile.open(des_file_name,std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    while(reader->Read(&chunk)){
        data = chunk.buffer();
        outfile.write(data,data.length());
    }
    //tellp 用于返回写入位置
    //tellg 则用于返回读取位置
    long pos = outfile.tellp();
    outfile.close();
    response->set_length(pos); 
    return Status::OK;
}


void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    TransferFileImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}


