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
				UIFactory(const std::string & name, const messages::TypeMapperHandle& mapper );
				void addButton(const std::string& name, const ButtonAction& action) const;
				void addToggle(const std::string& name, const ToggleAction & action, const bool defaultState = false) const;
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