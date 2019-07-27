#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "stream.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using std::cout;
using std::endl;

class streamServiceImpl final : public Greeter::Service
{

    // 服务端流式
    // 服务端给客户端返回一个流式数据
    Status GetStream(ServerContext *context, const StreamReqData *request, ServerWriter<StreamResData> *writer) override
    {
        std::string req = request->req_data();
        cout << "req::" << req << endl;
        int i = 0;
        StreamResData response;
        char msg[3][256] = {"1111", "2222", "3333"};
        for (i = 0; i < 3; i++)
        {
            response.set_res_data(msg[i]);
            writer->Write(response);
        }
        return Status::OK;
    }

    // 客户端流式
    // 服务端接收客户端一个流式数据
    Status PutStream(ServerContext *context, ServerReader<StreamReqData> *reader, StreamResData *response) override
    {
        StreamReqData request;
        while (reader->Read(&request))
        {
            std::string req = request.req_data();
            cout << "req::" << req << endl;
        }
        response->set_res_data("recv ok");
        return Status::OK;
    }

    //双向流
    // 一个双向流式 RPC 是双方使用 读写流 去发送一个消息序列
    // 两个流独立操作，client 和 server 可以以任意顺序读写
    // 比如，服务器可以在写入响应前等待接收所有的客户端消息。或者可以交替的读取和写入消息，
    // 或者其它读写顺序，每个流中的消息顺序被保留
    Status AllStream(ServerContext *context, ServerReaderWriter<StreamResData, StreamReqData> *stream) override
    {

        // 这里的示例是 先读取所有客户端输入，再返回流到客户端
        StreamReqData request;
        while (stream->Read(&request))
        {
            std::string req = request.req_data();
            cout << "req::" << req << endl;
        }

        StreamResData response;
        char msg[3][256] = {"1111", "2222", "3333"};
        for (i = 0; i < 3; i++)
        {
            response.set_res_data(msg[i]);
            writer->Write(response);
        }
        return Status::OK;
    }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    streamServiceImpl service;

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
