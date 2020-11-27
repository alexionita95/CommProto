#pragma once
#include <Socket.h>
#include <winsock2.h>

namespace commproto {
namespace socketlib {

	enum class WsaStartupResult
	{
		AlreadyStarted,
		StartupSucceeded,
		StartupFailed
	};

    enum class Mode : uint8_t {
        Unassigned,
        Server,
        Client
    };

    class SocketImpl : public Socket {
    public:
        SocketImpl();
		~SocketImpl();

        uint32_t sendBytes(const Message& message) override;
        uint32_t receive(Message& message, uint32_t size) override;
        bool initClient(const std::string& addr, const uint32_t port) override;
        bool initServer(const uint32_t port) override;
        SocketHandle acceptNext() override; // blocking
        uint32_t pollSocket() override;
        char readByte() override;

    private:
        //returns a ready socket from acceptNext()
        SocketImpl(SOCKET handle, const Mode mode, const bool initialized);
        Mode socketMode;
        bool isInitialized;
		SOCKET socketHandle;

	    static WsaStartupResult startWsa();
	    static void stopWsa();
		static bool isWsaStarted;
    };
} // namespace socket
} // namespace commproto
