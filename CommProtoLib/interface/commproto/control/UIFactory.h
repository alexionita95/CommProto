#ifndef UIFACTORY_H
#define UIFACTORY_H
#include <commproto/control/UIController.h>
#include <commproto/control/Button.h>
#include <commproto/sockets/Socket.h>
#include <commproto/messages/MessageMapper.h>
#include <commproto/control/Toggle.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint 
		{
			class UIFactory
			{
			public:
				UIFactory(const std::string & name, const messages::TypeMapperHandle& mapper , const sockets::SocketHandle& socket);
				uint32_t addButton(const std::string& name, const ButtonAction& action) const;
				uint32_t addToggle(const std::string& name, const ToggleAction & action, const bool defaultState = false) const;
				uint32_t addLabel(const std::string& name, const std::string & text) const;
				NotificationHandle addNotification(const std::string& name) const;
				UIControllerHandle build() const;
			private:
				UIControllerHandle controller;
			};
		}

		namespace ux
		{
			class UIFactory
			{
			public:
				UIFactory(const std::string & name, const std::string & connectionName, const messages::TypeMapperHandle& mapper, const sockets::SocketHandle& socket, const uint32_t id);
				UIControllerHandle build() const;
			private:
				UIControllerHandle controller;
			};
		}
	}
}


#endif // UIFACTORY_H