#ifndef COMMPROTO_SOCKET_H
#define COMMPROTO_SOCKET_H
#include <commproto/common/Common.h>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace commproto {
namespace sockets {


    class Socket;
    using SocketHandle = std::shared_ptr<Socket>;
    class Socket {
    public:
	    virtual ~Socket() = default;
	    //sendBytes a byte array, blocking
        virtual int32_t sendBytes(const Message& message) = 0;

        //block the thread until the socket receives [size] bytes
        virtual int32_t receive(Message& message, const uint32_t size) = 0;

        //poll the socket to see how many bytes are buffered
        virtual int32_t pollSocket() = 0;

        //block the thread until single byte of data can be read from the socket
        virtual int readByte() = 0;

		//block the thread until single byte of data can be sent to the socket
		virtual int sendByte(const char byte) = 0;

        //initialize the socket as a client and connect it to [addr]:[port]
        virtual bool initClient(const std::string& addr, const uint32_t port) = 0;

        //initialize the socket as a server and bind it to [port]
        virtual bool initServer(const std::string& addr, const uint32_t port) = 0;

        //block the thread until a connection is available, then accept it
        virtual SocketHandle acceptNext() = 0;

		//check connection state
		virtual bool connected() = 0;

		//close the connection
        virtual void shutdown() = 0;

		//set the timeout for socket operation
		virtual void setTimeout(const uint32_t msec) = 0;
    };

} // namespace core
} // namespace commproto

#endif //COMMPROTO_SOCKET_H
