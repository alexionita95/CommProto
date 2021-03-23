#ifndef SINGLE_PROPERTY_MESSAGE_H
#define SINGLE_PROPERTY_MESSAGE_H


#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{

		/**
		 * \brief Implements a generic message type with a single property. In order to be used, MAKE_SINGLE_PROP_MESSAGE alongside DEFINE_DATA_TYPE macros must be used to generate a class.
		 * \tparam T
		 */
		template <typename T>
		class SinglePropertyMessage : public MessageBase
		{
		public:
			SinglePropertyMessage(const uint32_t id, const T& prop_) : MessageBase{ id }, prop{ prop_ }
			{

			}
			;
			BP_TYPE_DEFAULTS(SinglePropertyMessage);

			T prop;

			uint32_t getSize() const override
			{
				return MessageBase::getSize() + messages::sizeOf(prop);
			}
		};

	}

}


#define MAKE_SINGLE_PROP_MESSAGE(MESSAGE_NAME, TYPE)								\
class MESSAGE_NAME : public commproto::messages::SinglePropertyMessage<TYPE>		\
{																					\
public:																				\
	MESSAGE_NAME(uint32_t id, const TYPE& prop_)									\
		: SinglePropertyMessage(id, prop_)											\
	{																				\
	}																				\
	BP_TYPE_DEFAULTS(MESSAGE_NAME)													\
}

#endif// SINGLE_PROPERTY_MESSAGE_H