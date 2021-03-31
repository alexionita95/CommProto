#ifndef EMPTY_MESSAGE_H
#define EMPTY_MESSAGE_H


#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{

		/**
		 * \brief Implements a message type with a no properties. In order to be used, MAKE_EMPTY_MESSAGE alongside DEFINE_DATA_TYPE macros must be used to generate a class.
		 * \tparam T
		 */
		class EmptyMessage : public MessageBase
		{
		public:
			EmptyMessage(const uint32_t id) : MessageBase{ id }
			{

			}
			;
			BP_TYPE_DEFAULTS(EmptyMessage);

			uint32_t getSize() const override
			{
				return MessageBase::getSize();
			}
		};

	}

}


#define MAKE_EMPTY_MESSAGE(MESSAGE_NAME)						\
class MESSAGE_NAME : public commproto::messages::EmptyMessage   \
{																\
public:															\
	MESSAGE_NAME(uint32_t id)									\
		: EmptyMessage(id)								        \
	{													    	\
	}															\
	BP_TYPE_DEFAULTS(MESSAGE_NAME)								\
}

#endif// SINGLE_PROPERTY_MESSAGE_H