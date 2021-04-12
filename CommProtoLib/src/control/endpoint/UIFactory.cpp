#include <commproto/control/endpoint/UIFactory.h>
#include "ButtonImpl.h"
#include "UIControllerImpl.h"
#include "IdProvider.h"
#include "NotificationImpl.h"
#include "LabelImpl.h"
#include "ToggleImpl.h"

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

			ButtonHandle UIFactory::addButton(const std::string& name, const ButtonAction& action) const
			{
				
				ButtonHandle button = std::make_shared<ButtonImpl>(name, controller->reserveId(), controller->getIdProvider().buttonId, action);
				controller->addControl(button);
				return button;
			}

			ToggleHandle UIFactory::addToggle(const std::string& name, const ToggleAction& action, const bool defaultState) const
			{
				ToggleHandle toggle = std::make_shared<ToggleImpl>(name, controller->reserveId(), controller->getIdProvider().toggleId, action, defaultState);
				controller->addControl(toggle);
				return toggle;
			}

			LabelHandle UIFactory::addLabel(const std::string& name, const std::string& text) const
			{
				LabelHandle label = std::make_shared<LabelImpl>(name, controller->reserveId(), controller->getIdProvider().labelId, controller->getIdProvider().labelUpdateId, text, controller);
				controller->addControl(label);
				return label;
			}

			NotificationHandle UIFactory::addNotification(const std::string& name) const
			{
				NotificationHandle notif = std::make_shared<NotificationImpl>(name, controller->reserveId(), controller->getIdProvider().notificationId,controller->getIdProvider().displayNotificationId);
				controller->addNotification(notif);
				return notif;
			}

			UIControllerHandle UIFactory::build() const
			{
				return  controller;
			}
		}
	}
}
