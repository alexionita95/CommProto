#ifndef DOUBLE_PROPERTY_MESSAGE_H
#define DOUBLE_PROPERTY_MESSAGE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{

		/**
		* \brief Implements a generic message type with two properties. In order to be used, MAKE_DOUBLE_PROP_MESSAGE alongside DEFINE_DATA_TYPE macros must be used to generate a class.
		* \tparam T
		*/
		template <typename T, typename U>
		class DoublePropertyMessage : public MessageBase
		{
		public:
			DoublePropertyMessage(const uint32_t id, const T& prop_, const U& prop2_) : MessageBase{ id }, prop{ prop_ }, prop2{ prop2_ }
			{

			}
			;
			BP_TYPE_DEFAULTS(DoublePropertyMessage);

			T prop;
			U prop2;

			uint32_t getSize() const override
			{
				return MessageBase::getSize() + sizeOf(prop) + sizeOf(prop2);
			}
		};
	}

}


#define MAKE_DOUBLE_PROP_MESSAGE(MESSAGE_NAME, TYPE,TYPE2)							\
class MESSAGE_NAME : public commproto::messages::DoublePropertyMessage<TYPE,TYPE2>	\
{																					\
public:																				\
	MESSAGE_NAME(uint32_t id, const TYPE& prop_, const TYPE2& prop2_)				\
		: DoublePropertyMessage(id, prop_,prop2_)									\
	{																				\
	}																				\
	BP_TYPE_DEFAULTS(MESSAGE_NAME)													\
}

#endif// DOUBLE_PROPERTY_MESSAGE_H