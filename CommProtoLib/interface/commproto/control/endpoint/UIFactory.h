#ifndef ENDPOINT_UIFACTORY_H
#define ENDPOINT_UIFACTORY_H
#include <commproto/control/endpoint/UIController.h>
#include <commproto/control/endpoint/Button.h>
#include <commproto/sockets/Socket.h>
#include <commproto/messages/TypeMapper.h>
#include <commproto/control/endpoint/Toggle.h>
#include <commproto/control/endpoint/Label.h>

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
				ButtonHandle addButton(const std::string& name, const ButtonAction& action) const;
				ToggleHandle addToggle(const std::string& name, const ToggleAction & action, const bool defaultState = false) const;
				LabelHandle addLabel(const std::string& name, const std::string & text) const;
				NotificationHandle addNotification(const std::string& name) const;
				UIControllerHandle build() const;
			private:
				UIControllerHandle controller;
			};
		}

	}
}


#endif // ENDPOINT_UIFACTORY_H