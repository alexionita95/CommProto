#include "ToggleImpl.h"
#include "UxGenerator.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			ToggleHandler::ToggleHandler(const ux::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void ToggleHandler::handle(messages::MessageBase&& data)
			{
				ToggleMessage& msg = static_cast<ToggleMessage&>(data);
				ux::ToggleHandle button = std::make_shared<ux::ToggleImpl>(msg.prop2, msg.prop, controller->getIdProvider().toggleId, std::make_shared<ux::Generator>(*controller), msg.prop3);
				controller->addControl(button);
			}

			ToggleImpl::ToggleImpl(const std::string& name, uint32_t id, const uint32_t messageId, const ToggleAction& action_, const bool defaultState)
				: Toggle(name, id)
				, messageId{ messageId }
				, action{ action_ }
				, state{ defaultState }
			{
			}

			Message endpoint::ToggleImpl::serialize() const
			{
				return ToggleSerializer::serialize(std::move(ToggleMessage(messageId, id, name,state)));
			}

			void endpoint::ToggleImpl::toggle()
			{
				state = !state;
				action(state);
			}
		}
		namespace ux
		{
			ToggleHandler::ToggleHandler(const endpoint::UIControllerHandle& controller_)
				: controller{controller_}
			{
			}

			void ToggleHandler::handle(messages::MessageBase&& data)
			{
				ToggleMessage& msg = static_cast<ToggleMessage&>(data);
				if(!controller)
				{
					return;
				}
				auto toggle = controller->getControl(msg.prop);
				if(!toggle)
				{
					return;
				}
				auto cast = std::static_pointer_cast<endpoint::Toggle>(toggle);
				if(!cast)
				{
					return;
				}
				cast->toggle();
				
			}

			ToggleImpl::ToggleImpl(const std::string& name, uint32_t id, const uint32_t messageId, const GeneratorHandle& generator_, const bool defaultState)
				: Toggle(name, id)
				, messageId{ messageId }
				, generator{ generator_ }
				, state{ defaultState }
			{
			}

			std::string ToggleImpl::getUx()
			{
				return generator->generate(*this);
			}

			void ToggleImpl::toggle()
			{
				state = !state;
				ToggleMessage msg(messageId, id);
				generator->send(ToggleSerializer::serialize(std::move(msg)));
			}

			bool ToggleImpl::getState() const
			{
				return state;
			}
		}
	}
}
