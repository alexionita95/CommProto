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
			Dispatch();
			void sentTo(const std::string& name, const commproto::Message& msg);
			void sendAll(const commproto::Message& msg);
			void addConnection(const commproto::sockets::SocketHandle& connection);
			void removeConnection(const std::string& name);
			void registerChannel(const uint32_t id, const std::string & name);
			ConnectionHandle getConnection(const std::string& name) const;
			ConnectionHandle getConnection(const uint32_t id) const;

			~Dispatch();
		private:

			uint32_t getId(const std::string & name) const;

			std::map<uint32_t, ConnectionHandle> connections;
			std::map<std::string, uint32_t> connectionMapping;
			uint32_t idCounter;
		};
	}
}


#endif //COMMPROTO_DISPATCH_H