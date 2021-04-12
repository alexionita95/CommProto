#include "ButtonImpl.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
	
		namespace endpoint {

			PressButtonHandler::PressButtonHandler(const endpoint::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void PressButtonHandler::handle(messages::MessageBase&& data)
			{
				ux::PressButtonMessage& msg = static_cast<ux::PressButtonMessage&>(data);
				ControlHandle button = controller->getControl(msg.prop);

				if (!button)
				{
					return;
				}
				auto cast = std::static_pointer_cast<ButtonImpl>(button);
				if (!cast)
				{
					return;
				}
				cast->press();

			}
			ButtonImpl::ButtonImpl(const std::string& name, const uint32_t id_, const uint32_t buttonId_, const ButtonAction& action_)
				: Button{ name ,id_ }
				, action{ action_ }
				, buttonId{ buttonId_ }
			{
			}

			void ButtonImpl::press()
			{
				action();
			}

			Message ButtonImpl::serialize() const
			{
				return ButtonSerializer::serialize(std::move(ButtonMessage(buttonId, id, name)));
			}
		}

	}
}