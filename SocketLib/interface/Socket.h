#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace commproto {
namespace socketlib {

    using Message = std::vector<char>;
    class Socket;
    using SocketHandle = Socket*;
    class Socket {
    public:
        //send a byte array, blocking
        virtual uint32_t send(const Message& message) = 0;

        //block the thread until the socket receives [size] bytes
        virtual uint32_t receive(Message& message, const uint32_t size) = 0;

        //poll the socket to see how many bytes are buffered
        virtual uint32_t pollSocket() = 0;

        //block the thread until single byte of data can be read from the socket
        virtual char readByte() = 0;

        //initialize the socket as a client and connect it to [addr]:[port]
        virtual bool initClient(const std::string& addr, const uint32_t port) = 0;

        //initialize the socket as a server and bind it to [port]
        virtual bool initServer(const uint32_t port) = 0;

        //block the thread until a connection is available, then accept it
        virtual SocketHandle acceptNext() = 0;
    };

} // namespace core
} // namespace commproto
