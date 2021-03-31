#ifndef UIFACTORY_H
#define UIFACTORY_H
#include <commproto/control/UIController.h>
#include <commproto/control/Button.h>
#include "../messages/MessageMapper.h"
#include "commproto/sockets/Socket.h"

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