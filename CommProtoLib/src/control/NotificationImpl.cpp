#include "NotificationImpl.h"
#include "IdProvider.h"
#include "UxGenerator.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{

			void NotificationHandler::handle(messages::MessageBase&& data)
			{
				NotificationMessage& msg = static_cast<NotificationMessage&>(data);
				ux::NotificationHandle notification = std::make_shared<ux::NotificationImpl>(msg.prop2,
					msg.prop,
					controller->getIdProvider().notificationResponseId,
					msg.prop3,
					std::make_shared<ux::Generator>(*controller));
				controller->addNotification(notification);
			}

			void DisplayNotificationHandler::handle(messages::MessageBase&& data)
			{
				DisplayNotificationMessage& msg = static_cast<DisplayNotificationMessage&>(data);
				controller->displayNotification(msg.prop);
			}

			Message NotificationImpl::serialize() const
			{
				return NotificationSerializer::serialize(std::move(NotificationMessage(notifId,id,name,options)));
			}

			void NotificationImpl::addOption(const std::string& name)
			{
				options.push_back(name);
			}

			void NotificationImpl::setAction(const NotificationAction& action_)
			{
				action = action_;
			}

			void NotificationImpl::execute(const std::string& option)
			{
				if(!action)
				{
					return;
				}
				action(option);
			}

			Message NotificationImpl::serializeDisplay()
			{
				return DisplayNotificationSerializer::serialize(std::move(DisplayNotificationMessage(displayId, id)));
			}
		}
		namespace ux
		{
			void NotificationResponseHandler::handle(messages::MessageBase&& data)
			{
				NotificationResponseMessage& msg = static_cast<NotificationResponseMessage&>(data);

				endpoint::NotificationHandle notification = controller->getNotification(msg.prop);
				if (!notification)
				{
					return;
				}
				notification->execute(msg.prop2);

			}

			std::string NotificationImpl::getUx()
			{
				return generator->generate(this);
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