#ifndef ENDPOINT_NOTIFICATION_IMPL_H
#define ENDPOINT_NOTIFICATION_IMPL_H
#include <commproto/control/endpoint/Notification.h>
#include <commproto/control/endpoint/UIController.h>
#include <commproto/control/NotificationChains.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			class NotificationResponseHandler : public parser::Handler
			{
			public:
				NotificationResponseHandler(const endpoint::UIControllerHandle & controller_) : controller{ controller_ } {}
				void handle(messages::MessageBase&& data) override;
			private:
				UIControllerHandle controller;
			};


			class NotificationImpl : public Notification
			{
			public:
				NotificationImpl(const std::string& name, uint32_t id, const uint32_t notifId_, const uint32_t displayId_)
					: Notification(name, id)
					, notifId{ notifId_ }
					, displayId{ displayId_ }
				{
				}

				Message serialize() const override;
				void addOption(const std::string& name) override;
				void setAction(const NotificationAction& action) override;
				void execute(const std::string& option) override;
				Message serializeDisplay() override;
			private:
				const uint32_t notifId;
				const uint32_t displayId;
				std::vector<std::string> options;
				NotificationAction action;
			};
		}
	}

}

#endif //ENDPOINT_NOTIFICATION_IMPL_H