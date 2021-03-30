#include <commproto/control/UIFactory.h>
#include "ButtonImpl.h"
#include "UIControllerImpl.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint {
			UIFactory::UIFactory(const std::string & name)
				:controller{ std::make_shared<UIControllerImpl>(name) }
			{
			}

			void UIFactory::addButton(const std::string& name, const ButtonAction& action) const
			{
				controller->addControl(name, std::make_shared<ButtonImpl>(name, action));
			}

			UIControllerHandle UIFactory::build() const
			{
				return  controller;
			}
		}
	}
}
