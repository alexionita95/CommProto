#pragma once
#include <Socket.h>

namespace commproto {
namespace socketlib {

    enum class Mode : uint8_t {
        Unassigned,
        Server,
        Client
    };

    class SocketImpl : public Socket {
    public:
        SocketImpl();

        uint32_t send(const Message& message) override;
        uint32_t receive(Message& message, uint32_t size) override;
        bool initClient(const std::string& addr, const uint32_t port) override;
        bool initServer(const uint32_t port) override;
        SocketHandle acceptNext() override; // blocking
        uint32_t pollSocket() override;
        char readByte() override;

    private:
        //returns a ready socket from acceptNext()
        SocketImpl(const int32_t handle, const Mode mode, const bool initialized);
        bool initFd();
        Mode socketMode;
        bool isInitialized;
        int32_t socketHandle;
    };
} // namespace socket
} // namespace commproto
