#include "NotificationImpl.h"
#include "IdProvider.h"
#include "UxGenerator.h"

namespace commproto
{
	namespace control
	{
		
		namespace ux
		{
			void NotificationHandler::handle(messages::MessageBase&& data)
			{
				endpoint::NotificationMessage& msg = static_cast<endpoint::NotificationMessage&>(data);
				ux::NotificationHandle notification = std::make_shared<ux::NotificationImpl>(msg.prop2,
					msg.prop,
					controller->getIdProvider().notificationResponseId,
					msg.prop3,
					std::make_shared<ux::Generator>(*controller));
				controller->addNotification(notification);
			}

			void DisplayNotificationHandler::handle(messages::MessageBase&& data)
			{
				endpoint::DisplayNotificationMessage& msg = static_cast<endpoint::DisplayNotificationMessage&>(data);
				controller->displayNotification(msg.prop);
			}

			std::string NotificationImpl::getUx()
			{
				return generator->generate(*this);
			}

			void NotificationImpl::execute(const std::string& option)
			{
				Message msg = NotificationResponseSerializer::serialize(std::move(NotificationResponseMessage(executeId, id, option)));
				generator->send(std::move(msg));
			}

			std::vector<std::string> NotificationImpl::getOptions() const
			{
				return options;
			}
		}
	}
}