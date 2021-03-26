#include <commproto/endpoint/ChannelTerminationHandler.h>
#include <commproto/endpoint/EndpointChains.h>

namespace commproto
{
	namespace endpoint
	{
		ChannelTerminationHandler::ChannelTerminationHandler(const ChannelParserDelegatorHandle& delegator_)
			:delegator{delegator_}
		{

		}

		void ChannelTerminationHandler::handle(messages::MessageBase&& data)
		{
			ChannelTerminationMessage& msg = static_cast<ChannelTerminationMessage&>(data);
			delegator->notifyTermination(msg.prop);
		}

	}
}
