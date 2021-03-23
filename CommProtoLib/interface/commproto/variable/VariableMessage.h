#ifndef COMMPROTO_VARIABLE_MESSAGE_H
#define COMMPROTO_VARIABLE_MESSAGE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/variable/VariableBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace variable
	{
		class VariableMessage : public messages::MessageBase
		{
		public:
			ValueType valueType; //used to cast to the correct sub-type
			uint32_t index;

			VariableMessage(const uint32_t type, const ValueType valueType_, const uint32_t index)
				: MessageBase{ type }
				, valueType{ valueType_ }
				, index{ index }
			{}

			BP_TYPE_DEFAULTS(VariableMessage);

			uint32_t getSize() const override
			{
				return messages::MessageBase::getSize()
					+ sizeof(valueType)
					+ sizeof(index);
			}
		};


		template <typename T>
		class VariableMessageTemplated : public VariableMessage
		{
		public:
			T value;
			VariableMessageTemplated(const uint32_t type, const ValueType valueType_, const uint32_t index, T value_)
				: VariableMessage{ type , valueType_ , index }
				, value{ value_ }
			{

			}

			BP_TYPE_DEFAULTS(VariableMessageTemplated);


			uint32_t getSize() const override
			{
				return VariableMessage::getSize() + messages::sizeOf(value);
			}
		};

	}

	DEFINE_DATA_TYPE(variable::VariableMessage);
}

#endif //COMMPROTO_VARIABLE_MESSAGE_H