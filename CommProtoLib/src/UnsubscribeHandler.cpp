#include <commproto/service/UnsubscribeHandler.h>
#include <commproto/service/ServiceChains.h>

namespace commproto
{
namespace service
{
	UnsubscribeHandler::UnsubscribeHandler(Connection& connection) : connection{ connection }
	{
	}

	void UnsubscribeHandler::handle(messages::MessageBase&& data)
	{
		UnsubscribeMessage& message = static_cast<UnsubscribeMessage&>(data);
		connection.unsubscribe(message.prop);
	}
}
}
