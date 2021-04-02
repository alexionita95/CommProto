#include "ButtonImpl.h"
#include "UxGenerator.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			PressButtonHandler::PressButtonHandler(const endpoint::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void PressButtonHandler::handle(messages::MessageBase&& data)
			{
				PressButtonMessage& msg = static_cast<PressButtonMessage&>(data);
				endpoint::ControlHandle button = controller->getControl(msg.prop);

				if (!button)
				{
					return;
				}
				auto cast = std::static_pointer_cast<endpoint::ButtonImpl>(button);
				if (!cast)
				{
					return;
				}
				cast->press();

			}

			ButtonImpl::ButtonImpl(const std::string& name_, const uint32_t id_, const uint32_t pressId_, const GeneratorHandle& generator_)
				: Button{ name_,id_ }
				, generator{ generator_ }
				, pressId{ pressId_ }
			{

			}

			void ButtonImpl::press()
			{
				Message msg = PressButtonSerializer::serialize(std::move(PressButtonMessage(pressId, id)));
				generator->send(msg);
			}

			std::string ButtonImpl::getUx()
			{
				return generator->generate(*this);
			}
		}

		namespace endpoint {

			ButtonHandler::ButtonHandler(const ux::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void ButtonHandler::handle(messages::MessageBase&& data)
			{
				ButtonMessage& msg = static_cast<ButtonMessage&>(data);
				ux::ButtonHandle button = std::make_shared<ux::ButtonImpl>(msg.prop2, msg.prop, controller->getIdProvider().buttonId, std::make_shared<ux::Generator>(*controller));
				controller->addControl(button);
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