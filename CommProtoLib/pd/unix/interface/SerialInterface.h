#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H
#include <commproto/sockets/Socket.h>

namespace commproto
{
namespace serial
{
    class SerialInterface : public sockets::Socket
    {
    public:
        SerialInterface();
        int32_t sendBytes(const Message &message);
        int32_t receive(Message &message, const uint32_t size);
        int32_t pollSocket();
        int readByte();
        int sendByte(const char byte);
        bool initClient(const std::string &addr, const uint32_t port);
        bool initServer(const std::string &addr, const uint32_t port);
        sockets::SocketHandle acceptNext();
        bool connected();
        void shutdown();
        void setTimeout(const uint32_t msec);
        ~SerialInterface();
    private:
        int serialPort;
    };
}
}



#endif // SERIAL_INTERFACE_H
