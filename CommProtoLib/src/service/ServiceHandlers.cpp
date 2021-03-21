#include <commproto/service/ServiceHandlers.h>
#include <commproto/service/ServiceChains.h>

//TODO: CLEANUP - Split into multiple files

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

		SubscribeHandler::SubscribeHandler(Connection& connection_) : connection{ connection_ }
		{
		}

		void SubscribeHandler::handle(messages::MessageBase&& data)
		{
			SubscribeMessage& message = static_cast<SubscribeMessage&>(data);
			connection.subscribe(message.prop);
		}

		UnsubscribeHandler::UnsubscribeHandler(Connection& connection) : connection{connection}
		{
		}

		void UnsubscribeHandler::handle(messages::MessageBase&& data)
		{
			UnsubscribeMessage& message = static_cast<UnsubscribeMessage&>(data);
			connection.unsubscribe(message.prop);
		}

		void RegisterIdHandler::handle(messages::MessageBase&& data)
		{
			RegisterIdMessage& message = static_cast<RegisterIdMessage&>(data);
			SenderMapping::InitializeId(message.prop);
		}
	}
}
