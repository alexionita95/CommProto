#ifndef COMMPROTO_TYPE_MAPPER_H
#define COMMPROTO_TYPE_MAPPER_H
#include <string>
#include <stdint.h>
#include <TypeName.h>
#include <memory>

namespace commproto
{
	namespace data
	{
		class TypeMapper
		{
		public:
			template <typename T>
			uint32_t registerType();

			template <typename T>
			bool hasType();

			virtual ~TypeMapper() = default;
		protected:
			virtual uint32_t registerType(const std::string& type) = 0;
			virtual bool hasType(const std::string& type) = 0;
		};

		using TypeMapperHandle = std::shared_ptr<TypeMapper>;

		template <typename T>
		uint32_t TypeMapper::registerType()
		{
			return registerType(TypeName<T>::name());
		}

		template <typename T>
		bool TypeMapper::hasType()
		{
			return hasType(TypeName<T>::name());
		}

	}
}

#endif // COMMPROTO_TYPE_MAPPER_H