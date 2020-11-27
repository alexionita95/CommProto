#include <SocketImpl.h>
#include <stdio.h>
#include <thread>

using namespace commproto;
socketlib::Message makeMessage(const std::string& msg)
{
    socketlib::Message msgBytes;
    msgBytes.reserve(msg.size());
    for (uint8_t c : msg) {
        msgBytes.push_back(c);
    }
	msgBytes.push_back(0);
    return msgBytes;
}

std::string makeString(const socketlib::Message& msg)
{
	socketlib::Message copy(msg);
	copy.push_back(0);
    return std::string(msg.data());
}

int main(int argc, char*[])
{
    std::string stopMsg = "Die!";
    int port = 4242;
    std::string addr = "127.0.0.1";
    bool server = argc == 2;
    if (server) {

        socketlib::SocketHandle server = new socketlib::SocketImpl();
        printf("Attempting to bind a socket to port %d...\n", port);
        bool init = server->initServer(port);
        if (!init) {
            printf("An error occurred while attempting to bind to port %d.\n", port);
            return 0;
        }
        printf("Succesfully bound to port %d. Awaiting next connection...\n", port);

        while (true) {
            socketlib::SocketHandle client = server->acceptNext();

            if (client == nullptr) {
                printf("An error occurred while waiting for a connection.\n");
                return 0;
            }

            printf("Succesfully received a connection, sending messages.\n");
            socketlib::Message msg = makeMessage("Hello!");
            socketlib::Message msg2 = makeMessage(stopMsg);
            if (msg.size() != client->sendBytes(msg)) {
                printf("An error occurred while sending a message.\n");
                return 0;
            }
            if (msg2.size() != client->sendBytes(msg2)) {
                printf("An error occurred while sending a message.\n");
                return 0;
            }
            printf("Succesfully sent two messages, shutting down...\n");
        }
    } else {
        socketlib::SocketHandle client = new socketlib::SocketImpl();
        printf("Attempting to connect a client socket to %s:%d...\n", addr.c_str(), port);
        bool init = client->initClient(addr, port);
        if (!init) {
            printf("An error occurred while attempting to connect.\n");
            return 0;
        }
        bool keepRunning = true;
        while (keepRunning) {
            uint32_t bytes = client->pollSocket();
            if (bytes) {
                socketlib::Message recv;
                client->receive(recv, bytes);
                std::string msg = makeString(recv);
                printf("Received a message: \"%s\"\n", msg.c_str());
                if (msg.compare(stopMsg) == 0) {
                    printf("Received stop message, stopping...\n");
                    keepRunning = false;
                }
            }
        }
    }
    return 0;
}
