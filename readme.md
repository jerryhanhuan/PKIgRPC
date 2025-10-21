# PKI gRPC Services / PKI gRPC 服务

[English](#english) | [中文](#中文)

---

## English

### Overview

This project implements a comprehensive **Public Key Infrastructure (PKI) gRPC service suite** that provides cryptographic operations through gRPC APIs. The services include key generation, digital signatures, message digests, key wrapping, file transfer, and various cryptographic utilities essential for secure communications and data protection.

### Features

- **Multi-language Support**: C++ and Go implementations
- **Comprehensive PKI Services**: Key generation, digest calculation, key wrapping, file transfer
- **Modern Cryptography**: Support for SM2/SM4 (Chinese national standards), RSA, AES, DES, and various hash algorithms
- **gRPC Protocol**: High-performance, cross-platform RPC framework
- **Streaming Support**: Efficient handling of large data through streaming APIs
- **Production Ready**: Well-structured codebase with proper error handling

### Architecture

The project is organized into the following components:

```
PKIgRPC/
├── protos/          # Protocol Buffer definitions
├── cpp/             # C++ implementation
├── go/              # Go implementation  
└── .vscode/         # Development configuration
```

### Services Overview

#### 1. **Key Generation Service** (`keygenerate`)
- Generate symmetric keys (AES, DES, SM4)
- Generate asymmetric key pairs (RSA, EC/SM2)
- Support for various key sizes and elliptic curves

#### 2. **Digest Service** (`digest`)
- Calculate cryptographic hashes (MD5, SHA-1, SHA-256, SHA-384, SHA-512)
- Streaming support for large data processing
- Multiple algorithm support with case-insensitive names

#### 3. **Key Wrapping Service** (`wrapkey`)
- Secure key wrapping and unwrapping operations
- Support for multiple wrapping algorithms
- Key protection and secure key exchange

#### 4. **File Transfer Service** (`transferfile`)
- Secure file upload and download
- Streaming file transfer for large files
- Integrity verification

#### 5. **Message Search Service** (`msgsearch`)
- Message querying and search capabilities
- Efficient message retrieval

#### 6. **Random Service** (`random`)
- Cryptographically secure random number generation
- Various random data formats

#### 7. **Stream Service** (`stream`)
- Generic streaming data processing
- Bidirectional streaming support

### Build Instructions

#### C++ Implementation

```bash
cd cpp
make
```

For individual services:
```bash
cd cpp/keygenerate
make
./keygenerate_server  # Start server
./keygenerate_client  # Run client
```

#### Go Implementation

```bash
cd go
go mod tidy
go run main.go
```

### API Documentation

#### Key Generation Service

**Generate Keys:**
```protobuf
service KeyGenerateService {
    rpc Generate (KeyGenerateRequest) returns (KeyGenerateResponse) {}
}

message KeyGenerateRequest {
    string transformation = 1;  // Algorithm: AES, DES, SM4, RSA, EC
    int32 key_size = 2;        // Key size in bits
    string curve = 3;          // Curve name for EC algorithms
}
```

**Supported Algorithms:**
- **Symmetric**: AES (128/192/256), DES (64/128/192), SM4 (128)
- **Asymmetric**: RSA (1024/2048/4096/8192), EC (prime256v1, SM2)

#### Digest Service

**Calculate Digest:**
```protobuf
service DigestService {
    rpc Digest (stream DigestRequest) returns (DigestResponse) {}
}

message DigestRequest {
    string transformation = 1;  // Hash algorithm
    repeated bytes messages = 2; // Data to hash
}
```

**Supported Hash Algorithms:**
- MD5, SHA-1, SHA-224, SHA-256, SHA-384, SHA-512

### Usage Examples

#### C++ Client Example

```cpp
#include <grpcpp/grpcpp.h>
#include "keygenerate.grpc.pb.h"

int main() {
    // Create channel
    auto channel = grpc::CreateChannel("localhost:50051", 
                                     grpc::InsecureChannelCredentials());
    auto stub = KeyGenerateService::NewStub(channel);
    
    // Prepare request
    KeyGenerateRequest request;
    request.set_transformation("SM4");
    request.set_key_size(128);
    
    // Call service
    KeyGenerateResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Generate(&context, request, &response);
    
    if (status.ok()) {
        std::cout << "Generated key: " << response.secret_key() << std::endl;
    }
    
    return 0;
}
```

#### Protocol Buffer Compilation

```bash
# Generate C++ files
protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto
protoc --cpp_out=. *.proto

# Generate Go files  
protoc --go_out=. --go-grpc_out=. *.proto
```

### Development Setup

1. **Install Dependencies**
   - gRPC C++/Go libraries
   - Protocol Buffers compiler
   - OpenSSL (for cryptographic operations)

2. **Build Protocol Buffers**
   ```bash
   cd protos
   # Compile .proto files for your target language
   ```

3. **Run Services**
   ```bash
   # Start server
   ./keygenerate_server
   
   # Test with client
   ./keygenerate_client
   ```

### File Structure

```
PKIgRPC/
├── protos/
│   ├── keygenerate.proto    # Key generation service definition
│   ├── digest.proto         # Digest calculation service
│   ├── wrapkeys.proto       # Key wrapping service
│   ├── transferfile.proto   # File transfer service
│   └── ...                  # Other service definitions
├── cpp/
│   ├── keygenerate/         # Key generation C++ implementation
│   ├── digest/              # Digest service C++ implementation
│   ├── wrapkey/             # Key wrapping C++ implementation
│   └── ...                  # Other C++ services
└── go/
    └── stream/              # Go implementation examples
```

---

## 中文

### 概述

本项目实现了一套完整的**公钥基础设施(PKI) gRPC服务套件**，通过gRPC API提供密码学操作。服务包括密钥生成、数字签名、消息摘要、密钥包装、文件传输以及安全通信和数据保护所需的各种密码学工具。

### 特性

- **多语言支持**：C++和Go实现
- **完整的PKI服务**：密钥生成、摘要计算、密钥包装、文件传输
- **现代密码学**：支持SM2/SM4（中国国家标准）、RSA、AES、DES和各种哈希算法
- **gRPC协议**：高性能、跨平台RPC框架
- **流式支持**：通过流式API高效处理大数据
- **生产就绪**：结构良好的代码库，具有适当的错误处理

### 架构

项目组织为以下组件：

```
PKIgRPC/
├── protos/          # Protocol Buffer定义
├── cpp/             # C++实现
├── go/              # Go实现  
└── .vscode/         # 开发配置
```

### 服务概览

#### 1. **密钥生成服务** (`keygenerate`)
- 生成对称密钥（AES、DES、SM4）
- 生成非对称密钥对（RSA、EC/SM2）
- 支持各种密钥长度和椭圆曲线

#### 2. **摘要服务** (`digest`)
- 计算密码学哈希（MD5、SHA-1、SHA-256、SHA-384、SHA-512）
- 支持大数据流式处理
- 多算法支持，算法名称大小写不敏感

#### 3. **密钥包装服务** (`wrapkey`)
- 安全的密钥包装和解包装操作
- 支持多种包装算法
- 密钥保护和安全密钥交换

#### 4. **文件传输服务** (`transferfile`)
- 安全文件上传和下载
- 大文件流式传输
- 完整性验证

#### 5. **消息搜索服务** (`msgsearch`)
- 消息查询和搜索功能
- 高效消息检索

#### 6. **随机数服务** (`random`)
- 密码学安全的随机数生成
- 各种随机数据格式

#### 7. **流服务** (`stream`)
- 通用流式数据处理
- 双向流支持

### 编译说明

#### C++实现

```bash
cd cpp
make
```

针对单个服务：
```bash
cd cpp/keygenerate
make
./keygenerate_server  # 启动服务器
./keygenerate_client  # 运行客户端
```

#### Go实现

```bash
cd go
go mod tidy
go run main.go
```

### API文档

#### 密钥生成服务

**生成密钥：**
```protobuf
service KeyGenerateService {
    rpc Generate (KeyGenerateRequest) returns (KeyGenerateResponse) {}
}

message KeyGenerateRequest {
    string transformation = 1;  // 算法：AES、DES、SM4、RSA、EC
    int32 key_size = 2;        // 密钥长度（位）
    string curve = 3;          // EC算法的曲线名称
}
```

**支持的算法：**
- **对称算法**：AES (128/192/256)、DES (64/128/192)、SM4 (128)
- **非对称算法**：RSA (1024/2048/4096/8192)、EC (prime256v1、SM2)

#### 摘要服务

**计算摘要：**
```protobuf
service DigestService {
    rpc Digest (stream DigestRequest) returns (DigestResponse) {}
}

message DigestRequest {
    string transformation = 1;  // 哈希算法
    repeated bytes messages = 2; // 要计算哈希的数据
}
```

**支持的哈希算法：**
- MD5、SHA-1、SHA-224、SHA-256、SHA-384、SHA-512

### 使用示例

#### C++客户端示例

```cpp
#include <grpcpp/grpcpp.h>
#include "keygenerate.grpc.pb.h"

int main() {
    // 创建通道
    auto channel = grpc::CreateChannel("localhost:50051", 
                                     grpc::InsecureChannelCredentials());
    auto stub = KeyGenerateService::NewStub(channel);
    
    // 准备请求
    KeyGenerateRequest request;
    request.set_transformation("SM4");
    request.set_key_size(128);
    
    // 调用服务
    KeyGenerateResponse response;
    grpc::ClientContext context;
    grpc::Status status = stub->Generate(&context, request, &response);
    
    if (status.ok()) {
        std::cout << "生成的密钥: " << response.secret_key() << std::endl;
    }
    
    return 0;
}
```

#### Protocol Buffer编译

```bash
# 生成C++文件
protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto
protoc --cpp_out=. *.proto

# 生成Go文件  
protoc --go_out=. --go-grpc_out=. *.proto
```

### 开发环境设置

1. **安装依赖**
   - gRPC C++/Go库
   - Protocol Buffers编译器
   - OpenSSL（用于密码学操作）

2. **构建Protocol Buffers**
   ```bash
   cd protos
   # 为目标语言编译.proto文件
   ```

3. **运行服务**
   ```bash
   # 启动服务器
   ./keygenerate_server
   
   # 使用客户端测试
   ./keygenerate_client
   ```

### 文件结构

```
PKIgRPC/
├── protos/
│   ├── keygenerate.proto    # 密钥生成服务定义
│   ├── digest.proto         # 摘要计算服务
│   ├── wrapkeys.proto       # 密钥包装服务
│   ├── transferfile.proto   # 文件传输服务
│   └── ...                  # 其他服务定义
├── cpp/
│   ├── keygenerate/         # 密钥生成C++实现
│   ├── digest/              # 摘要服务C++实现
│   ├── wrapkey/             # 密钥包装C++实现
│   └── ...                  # 其他C++服务
└── go/
    └── stream/              # Go实现示例
```