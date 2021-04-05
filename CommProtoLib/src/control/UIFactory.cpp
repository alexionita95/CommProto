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
			UIFactory::UIFactory(const std::string & name, const messages::TypeMapperHandle& mapper, const sockets::SocketHandle& socket)
				:controller{ std::make_shared<UIControllerImpl>(name, mapper, socket) }
			{
			}

			uint32_t UIFactory::addButton(const std::string& name, const ButtonAction& action) const
			{
				uint32_t id = controller->reserveId();
				controller->addControl(std::make_shared<ButtonImpl>(name,id, controller->getIdProvider().buttonId, action));
				return id;
			}

			uint32_t UIFactory::addToggle(const std::string& name, const ToggleAction& action, const bool defaultState) const
			{
				uint32_t id = controller->reserveId();
				controller->addControl(std::make_shared<ToggleImpl>(name, id, controller->getIdProvider().toggleId, action, defaultState));
				return id;
			}

			uint32_t UIFactory::addLabel(const std::string& name, const std::string& text) const
			{
				uint32_t id = controller->reserveId();
				controller->addControl(std::make_shared<LabelImpl>(name, id, controller->getIdProvider().labelId, controller->getIdProvider().labelUpdateId, text, controller));
				return id;
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
