#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include <commproto/control/Control.h>
#include <commproto/control/ControlCollection.h>
#include <commproto/control/Notification.h>

namespace commproto
{
	namespace control {
		namespace endpoint {

			class IdProvider;

			class UIController : public Control, public ControlCollection
			{
			public:
				UIController(const std::string & name) : Control{ name, 0 } {}
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
				virtual void addNotification(const NotificationHandle& notification) = 0;
				virtual NotificationHandle getNotification(const uint32_t id) const = 0;
				virtual void displayNotification(const uint32_t id) const = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}

		namespace ux {

			class IdProvider;


			class UIController : public Control, public ControlCollection
			{
			public:
				UIController(const std::string & name) : Control{ name,0 } {}
				virtual std::string getConnectionName() = 0;
				virtual uint32_t getConnectionId() = 0;
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
				virtual void addNotification(const NotificationHandle& notification) = 0;
				virtual NotificationHandle getNotification(const uint32_t id) const = 0;
				virtual void displayNotification(const uint32_t id) = 0;
				virtual bool hasNotifications() const = 0;
				virtual std::string getNotifications() = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}
	}
}


#endif // UICONTROLLER_H