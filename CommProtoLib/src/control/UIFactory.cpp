#include <commproto/control/UIFactory.h>
#include "ButtonImpl.h"
#include "UIControllerImpl.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			UIFactory::UIFactory(const std::string & name, const messages::TypeMapperHandle& mapper)
				:controller{ std::make_shared<UIControllerImpl>(name, mapper) }
			{
			}

			void UIFactory::addButton(const std::string& name, const ButtonAction& action) const
			{
				controller->addControl(std::make_shared<ButtonImpl>(name,controller->reserveId(), controller->getIdProvider().buttonId, action));
			}

			void UIFactory::addToggle(const std::string& name, const ToggleAction& action, const bool defaultState) const
			{
				controller->addControl(std::make_shared<ToggleImpl>(name, controller->reserveId(), controller->getIdProvider().toggleId, action, defaultState));
			}

			UIControllerHandle UIFactory::build() const
			{
				return  controller;
			}
		}

		namespace ux
		{
			UIFactory::UIFactory(const std::string & name, const std::string & connectionName, const messages::TypeMapperHandle& mapper, const sockets::SocketHandle& socket, const uint32_t id)
				: controller{ std::make_shared<UIControllerImpl>(name,connectionName, mapper,socket,id) }
			{
			}

			UIControllerHandle UIFactory::build() const
			{
				return  controller;
			}
		}
	}
}
