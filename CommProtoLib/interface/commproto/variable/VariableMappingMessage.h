#ifndef COMMPROTO_VARIABLE_MAPPING_MESSAGE_H
#define COMMPROTO_VARIABLE_MAPPING_MESSAGE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/variable/VariableBase.h>
#include <commproto/messages/MessageName.h>
#include <commproto/parser/ByteStream.h>

namespace commproto
{
	namespace variable
	{
		class VariableMappingMessage : public messages::MessageBase
		{
		public:
			std::string name;
			uint32_t index;

			VariableMappingMessage(const uint32_t type, const std::string & name_, const uint32_t index)
				: MessageBase{ type }
				, name{ name_ }
				, index{ index }
			{}

			BP_TYPE_DEFAULTS(VariableMappingMessage);

			uint32_t getSize() const override
			{
				return messages::MessageBase::getSize()
					+ messages::sizeOf(name)
					+ sizeof(index);
			}
		};
		class VariableMappingSerializer
		{
		public:
			static Message serialize(messages::MessageBase && data)
			{
				VariableMappingMessage& value = static_cast<VariableMappingMessage&>(data);
				parser::ByteStream stream;
				stream.writeHeader(data);
				stream.write(value.name);
				stream.write(value.index);
				return stream.getStream();

			}
		};

	}


	

	namespace messages
	{
		DEFINE_DATA_TYPE(variable::VariableMappingMessage);
	}
}

#endif //COMMPROTO_VARIABLE_MAPPING_MESSAGE_H