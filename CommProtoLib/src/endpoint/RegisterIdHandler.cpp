
#include <commproto/endpoint/EndpointChains.h>
#include <commproto/endpoint/RegisterIdHandler.h>

namespace commproto
{
	namespace endpoint
	{

		void RegisterIdHandler::handle(messages::MessageBase&& data)
		{
			RegisterIdMessage& message = static_cast<RegisterIdMessage&>(data);
			SenderMapping::InitializeId(message.prop);
		}

	}
}
