#include <commproto/service/Dispatch.h>

namespace commproto {
	namespace service {
		Dispatch::Dispatch()
			: idCounter{1}
		{
		}

		void Dispatch::sentTo(const std::string& name, const Message& msg)
		{
			const uint32_t id = getId(name);
			if (connections.find(id) == connections.end())
			{
				return;
			}
			connections[id]->send(msg);
		}

		void Dispatch::sendAll(const Message& msg)
		{
			for (auto it = connections.begin(); it != connections.end(); ++it)
			{
				it->second->send(msg);
			}
		}

		void Dispatch::addConnection(const sockets::SocketHandle& connection)
		{
			const uint32_t connectionId = idCounter++;
			ConnectionHandle newCon = std::make_shared<Connection>(connectionId, connection, this);
			connections.insert({ connectionId,  newCon });
			newCon->start();
		}

		void Dispatch::removeConnection(const std::string& name)
		{
			const uint32_t id = getId(name);

			auto it = connections.find(id);

			if (it == connections.end())
			{
				return;
			}

			connections.erase(it);
		}

		void Dispatch::registerChannel(const uint32_t id, const std::string& name)
		{
			auto connection = connections.find(id);
			if(connectionMapping.find(name) != connectionMapping.end() || connection == connections.end())
			{
				return;
			}

			connectionMapping.insert({ name,id });
			connection->second->setName(name);
		}

		ConnectionHandle Dispatch::getConnection(const std::string& name) const
		{
			return getConnection(getId(name));
		}

		ConnectionHandle Dispatch::getConnection(const uint32_t id) const
		{
			auto it = connections.find(id);

			if (it == connections.end())
			{
				return ConnectionHandle();
			}

			return it->second;
		}

		Dispatch::~Dispatch()
		{
			for (auto connection : connections)
			{
				connection.second->stop();
				connection.second->clearSubscriptions();
			}
			connections.clear();
		}

		uint32_t Dispatch::getId(const std::string& name) const
		{
			auto idIt = connectionMapping.find(name);
			if (idIt == connectionMapping.end())
			{
				return 0;
			}
			return idIt->second;
		}
	}
}