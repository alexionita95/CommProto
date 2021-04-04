#include <commproto/messages/MessageMapper.h>

#include "../parser/TypeMapperImpl.h"

namespace commproto
{
	namespace messages
	{

		TypeMapperHandle TypeMapperFactory::build(const sockets::SocketHandle& socket)
		{
			TypeMapperObserverHandle observer = std::make_shared<TypeMapperObserver>(socket);
			return std::make_shared<messages::TypeMapperImpl>(observer);
		}
	}
}