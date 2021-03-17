#ifndef COMMPROTO_SERVICE_CHANNEL_NAME_MESSAGE_H
#define COMMPROTO_SERVICE_CHANNEL_NAME_MESSAGE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace service
	{
		class RegisterChannelMessage : public messages::MessageBase
		{
		public:
			RegisterChannelMessage(const uint32_t id, const std::string& name_) : MessageBase{id}, name{ name_ }
			{
				
			}
			;
			BP_TYPE_DEFAULTS(RegisterChannelMessage);

			const std::string name;

			uint32_t getSize() const override
			{
				return MessageBase::getSize() + messages::sizeOf(name);
			}
		};
	}



	namespace messages
	{
		DEFINE_DATA_TYPE(service::RegisterChannelMessage);
	}
}

#endif// COMMPROTO_SERVICE_CHANNEL_NAME_MESSAGE_H