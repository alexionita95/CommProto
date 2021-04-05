#include "LabelImpl.h"
#include "UxGenerator.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			LabelHandler::LabelHandler(const ux::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void LabelHandler::handle(messages::MessageBase&& data)
			{
				LabelMessage& message = static_cast<LabelMessage&>(data);
				ux::ControlHandle label = std::make_shared<ux::LabelImpl>(message.prop2, message.prop, message.prop3, std::make_shared<ux::Generator>(*controller));
				controller->addControl(label);
			}

			LabelUpdateHandler::LabelUpdateHandler(const ux::UIControllerHandle& controller_)
				: controller{ controller_ }
			{
			}

			void LabelUpdateHandler::handle(messages::MessageBase&& data)
			{
				LabelUpdateMessage& message = static_cast<LabelUpdateMessage&>(data);
				ux::ControlHandle label = controller->getControl(message.prop);
				if (!label)
				{
					return;
				}

				ux::LabelHandle cast = std::static_pointer_cast<ux::LabelImpl>(label);
				if (!cast)
				{
					return;
				}
				cast->setText(message.prop2);
			}

			LabelImpl::LabelImpl(const std::string& name, uint32_t id, const uint32_t messageId_, const uint32_t updateMsgId_, const std::string& defaultText, const UIControllerHandle& controller_)
				: Label(name, id)
				, messageId{ messageId_ }
				, updateId{ updateMsgId_ }
				, text{ defaultText }
				, controller{ controller_ }
			{
			}

			Message LabelImpl::serialize() const
			{
				return LabelSerializer::serialize(std::move(LabelMessage(messageId, id, name, text)));
			}

			void LabelImpl::setText(const std::string& text_)
			{
				text = text_;
				controller->send(LabelUpdateSerializer::serialize(std::move(LabelUpdateMessage(updateId, id, text))));
			}
		}
		namespace ux
		{
			LabelImpl::LabelImpl(const std::string& name, uint32_t id, const std::string& text_, const GeneratorHandle & generator_)
				: Label(name, id)
				, text{ text_ }
				, generator{ generator_ }
			{
			}

			std::string LabelImpl::getUx()
			{
				return generator->generate(*this);
			}

			void LabelImpl::setText(const std::string& text_)
			{
				text = text_;
				generator->notifyUpdate();
			}

			std::string LabelImpl::getText() const
			{
				return text;
			}
		}
	}
}
