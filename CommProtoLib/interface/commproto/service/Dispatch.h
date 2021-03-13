#ifndef COMMPROTO_DISPATCH_H
#define COMMPROTO_DISPATCH_H
#include <map>
#include <thread>
#include <commproto/sockets/Socket.h>
#include <commproto/service/Connection.h>

namespace commproto {
	namespace service {

		class Dispatch
		{
		public:
			void sentTo(const std::string& name, const commproto::Message& msg);
			void sendAll(const commproto::Message& msg);
			void addConnection(const std::string& name, const commproto::sockets::SocketHandle& connection);
			void removeConnection(const std::string& name);
			~Dispatch();
		private:
			std::map<std::string, ConnectionHandle> connections;
		};
	}
}


#endif //COMMPROTO_DISPATCH_H