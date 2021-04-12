#include "NotificationImpl.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{

			void NotificationResponseHandler::handle(messages::MessageBase&& data)
			{
				ux::NotificationResponseMessage& msg = static_cast<ux::NotificationResponseMessage&>(data);

				NotificationHandle notification = controller->getNotification(msg.prop);
				if (!notification)
				{
					return;
				}
				notification->execute(msg.prop2);

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
	}
}