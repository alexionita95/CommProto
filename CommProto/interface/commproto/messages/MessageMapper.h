#ifndef COMMPROTO_TYPE_MAPPER_H
#define COMMPROTO_TYPE_MAPPER_H
#include <string>
#include <stdint.h>
#include <memory>

#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace messages
	{
		class MessageTypeMapper
		{
		public:
			template <typename T>
			uint32_t registerType();

			template <typename T>
			bool hasType();

			virtual ~MessageTypeMapper() = default;
		protected:
			virtual uint32_t registerType(const std::string& type) = 0;
			virtual bool hasType(const std::string& type) = 0;
		};

		using TypeMapperHandle = std::shared_ptr<MessageTypeMapper>;

		template <typename T>
		uint32_t MessageTypeMapper::registerType()
		{
			return registerType(MessageName<T>::name());
		}

		template <typename T>
		bool MessageTypeMapper::hasType()
		{
			return hasType(MessageName<T>::name());
		}

	}
}

#endif // COMMPROTO_TYPE_MAPPER_H