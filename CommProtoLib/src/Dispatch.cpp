#include <commproto/service/Dispatch.h>

namespace commproto {
	namespace service {

		void Dispatch::sentTo(const std::string& name, const Message& msg)
		{
			if (connections.find(name) == connections.end())
			{
				return;
			}
			connections[name]->send(msg);
		}

		void Dispatch::sendAll(const Message& msg)
		{
			for (auto it = connections.begin(); it != connections.end(); ++it)
			{
				it->second->send(msg);
			}
		}

		void Dispatch::addConnection(const std::string& name, const sockets::SocketHandle& connection)
		{
			connections.insert({ name, std::make_shared<Connection>(name, connection, this) });
			connections[name]->start();
		}

		void Dispatch::removeConnection(const std::string& name)
		{
			auto it = connections.find(name);

			if (it == connections.end())
			{
				return;
			}

			connections.erase(it);
		}

		Dispatch::~Dispatch()
		{
			for (auto connection : connections)
			{
				connection.second->stop();
			}
			connections.clear();
		}

	}
}