#include <commproto/endpoint/ChannelMappingHandler.h>
#include <commproto/logger/Logging.h>
#include <commproto/endpoint/EndpointChains.h>

namespace commproto
{
	namespace endpoint
	{


		ChannelMappingHandler::ChannelMappingHandler(const endpoint::ChannelParserDelegatorHandle& delegator_)
			: delegator{delegator_}
		{
		}

		void ChannelMappingHandler::handle(messages::MessageBase&& data)
		{
			ChannelMappingMessage& message = static_cast<ChannelMappingMessage&>(data);
			LOG_INFO("Registed a mapping \"%s\" = %d", message.prop.c_str(), message.prop2);
			delegator->notifyMapping(message.prop, message.prop2);			
		}
    }
}