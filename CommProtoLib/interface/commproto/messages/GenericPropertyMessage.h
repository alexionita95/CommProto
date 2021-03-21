#ifndef COMMPROTO_PROPERTY_MESSAGE_H
#define COMMPROTO_PROPERTY_MESSAGE_H

//TODO: CLEANUP - Split into multiple files

#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{

		/**
		 * \brief Implements a generic message type with a single property. In order to be used, DEFINE_DATA_TYPE with a using alias of this must be used for parser mapping.
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


		/**
		* \brief Implements a generic message type with two properties. In order to be used, DEFINE_DATA_TYPE with a using alias of this must be used for parser mapping.
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
			U prop3;

			uint32_t getSize() const override
			{
				return MessageBase::getSize() + sizeOf(prop) + sizeOf(prop2) + sizeOf(prop3);
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

#endif// COMMPROTO_SINGLE_PROPERTY_MESSAGE_H