#ifndef ENDPOINT_UICONTROLLER_IMPL_H
#define ENDPOINT_UICONTROLLER_IMPL_H

#include <commproto/control/endpoint/UIController.h>
#include <commproto/sockets/Socket.h>
#include <map>
#include "IdProvider.h"

namespace commproto
{
	namespace control {
		namespace endpoint
		{
			class UIControllerImpl : public UIController
			{
			public:
				UIControllerImpl(const std::string& name, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_);
				void addControl(const ControlHandle& control) override;
				Message serialize() const override;
				~UIControllerImpl();
				IdProvider& getIdProvider() override;
				ControlHandle getControl(const uint32_t id) override;
				uint32_t reserveId() override;
				void clear() override;
				void send(Message msg) override;
				void addNotification(const NotificationHandle& notification) override;
				NotificationHandle getNotification(const uint32_t id) const override;
				void displayNotification(const uint32_t id) const override;
			private:
				std::map<uint32_t, ControlHandle> controls;
				std::map<uint32_t, NotificationHandle> notifications;
				IdProvider provider;
				uint32_t idCounter;
				sockets::SocketHandle socket;
			};
		}
	}
}


#endif // ENDPOINT_UICONTROLLER_IMPL_H