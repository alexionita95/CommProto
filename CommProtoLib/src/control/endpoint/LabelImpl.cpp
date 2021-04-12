#include "LabelImpl.h"
#include <commproto/control/LabelChains.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{

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
	}
}
