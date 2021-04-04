#ifndef TRIPLE_PROPERTY_MESSAGE_H
#define TRIPLE_PROPERTY_MESSAGE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{

		/**
		* \brief Implements a generic message type with three properties. In order to be used, DEFINE_DATA_TYPE with a using alias of this must be used for parser mapping.
		* \tparam T
		*/
		template <typename T, typename U, typename V>
		class TriplePropertyMessage : public MessageBase
		{
		public:
			TriplePropertyMessage(const uint32_t id, const T& prop_, const U& prop2_, const V& prop3_) : MessageBase{ id }, prop{ prop_ }, prop2{ prop2_ }, prop3{ prop3_ }
			{

			}
			;
			BP_TYPE_DEFAULTS(TriplePropertyMessage);

			T prop;
			U prop2;
			V prop3;

			uint32_t getSize() const override
			{
				return MessageBase::getSize() + sizeOf(prop) + sizeOf(prop2) + sizeOf(prop3);
			}
		};
	}

}

#define MAKE_TRIPLE_PROP_MESSAGE(MESSAGE_NAME, TYPE,TYPE2,TYPE3)							\
class MESSAGE_NAME : public commproto::messages::TriplePropertyMessage<TYPE,TYPE2,TYPE3>	\
{																							\
public:																						\
	MESSAGE_NAME(uint32_t id, const TYPE& prop_, const TYPE2& prop2_,const TYPE3& prop3_)	\
		: TriplePropertyMessage(id, prop_,prop2_,prop3_)									\
	{																						\
	}																						\
	BP_TYPE_DEFAULTS(MESSAGE_NAME)															\
}

#endif// TRIPLE_PROPERTY_MESSAGE_H