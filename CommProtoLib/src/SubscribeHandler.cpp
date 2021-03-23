#include <commproto/service/SubscribeHandler.h>
#include <commproto/service/ServiceChains.h>

namespace commproto
{
namespace service
{
	SubscribeHandler::SubscribeHandler(Connection& connection_) : connection{ connection_ }
	{
	}

	void SubscribeHandler::handle(messages::MessageBase&& data)
	{
		SubscribeMessage& message = static_cast<SubscribeMessage&>(data);
		connection.subscribe(message.prop);
	}
}
}
