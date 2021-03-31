#include <commproto/control/UIFactory.h>
#include "ButtonImpl.h"
#include "UIControllerImpl.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint {
			UIFactory::UIFactory(const std::string & name, const messages::TypeMapperHandle& mapper)
				:controller{ std::make_shared<UIControllerImpl>(name, mapper) }
			{
			}

			void UIFactory::addButton(const std::string& name, const ButtonAction& action) const
			{
				controller->addControl(std::make_shared<ButtonImpl>(name,controller->getIdProvider().buttonId ,action));
			}

			UIControllerHandle UIFactory::build() const
			{
				return  controller;
			}
		}
	}
}
