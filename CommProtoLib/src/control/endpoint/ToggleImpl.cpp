#include "ToggleImpl.h"
#include "IdProvider.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			ToggleHandler::ToggleHandler(const endpoint::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void ToggleHandler::handle(messages::MessageBase&& data)
			{
				ToggleMessage& msg = static_cast<ToggleMessage&>(data);
				if (!controller)
				{
					return;
				}
				auto toggle = controller->getControl(msg.prop);
				if (!toggle)
				{
					return;
				}
				auto cast = std::static_pointer_cast<endpoint::Toggle>(toggle);
				if (!cast)
				{
					return;
				}
				cast->toggle();

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
	}
}
