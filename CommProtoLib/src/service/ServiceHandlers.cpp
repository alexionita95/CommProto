#include <commproto/service/ServiceHandlers.h>
#include <commproto/service/ServiceChains.h>

namespace commproto
{
	namespace service
	{
		RegisterChannelHandler::RegisterChannelHandler(Dispatch* dispatch_, const uint32_t id_)
			: dispatch{ dispatch_ }
			, id{ id_ }
		{
		}

		void RegisterChannelHandler::handle(messages::MessageBase&& data)
		{
			if (!dispatch)
			{
				return;
			}
			RegisterChannelMessage& message = static_cast<RegisterChannelMessage&>(data);
			dispatch->registerChannel(id, message.prop);
		}

		void RegisterIdHandler::handle(messages::MessageBase&& data)
		{
			RegisterIdMessage& message = static_cast<RegisterIdMessage&>(data);
			SenderMapping::InitializeId(message.prop);
		}
	}
}
