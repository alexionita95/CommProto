#ifndef COMMPROTO_TYPE_MAPPER_OBSERVER_H
#define COMMPROTO_TYPE_MAPPER_OBSERVER_H
#include <commproto/sockets/Socket.h>
#include "MappingType.h"

namespace commproto
{
	namespace messages
	{

		class TypeMapperObserver
		{
		public:
			TypeMapperObserver(const sockets::SocketHandle & socket_)
				: socket{ socket_ }
			{

			}

			void notify(const std::string & name, uint32_t id)
			{
				if(id == 0 || name == "")
				{
					return;
				}
				socket->sendBytes(serializer.serialize(std::move(MappingType(name, id))));
			}
		private:
			sockets::SocketHandle socket;
			MappingTypeSerializer serializer;
		};
		using TypeMapperObserverHandle = std::shared_ptr<TypeMapperObserver>;
	}
}
#endif // COMMPROTO_TYPE_MAPPER_OBSERVER_H