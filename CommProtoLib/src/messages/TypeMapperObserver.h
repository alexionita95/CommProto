#ifndef COMMPROTO_TYPE_MAPPER_OBSERVER_H
#define COMMPROTO_TYPE_MAPPER_OBSERVER_H
#include <commproto/sockets/Socket.h>
#include <commproto/parser/MappingType.h>
#include "commproto/logger/Logging.h"

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
				LOG_DEBUG("Observed a new type mapping, %s - %d", name.c_str(), id);
				socket->sendBytes(MappingTypeSerializer::serialize(std::move(MappingType(name, id))));
			}
		private:
			sockets::SocketHandle socket;
		};
		using TypeMapperObserverHandle = std::shared_ptr<TypeMapperObserver>;
	}
}
#endif // COMMPROTO_TYPE_MAPPER_OBSERVER_H