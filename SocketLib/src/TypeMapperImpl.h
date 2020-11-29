#ifndef COMMPROTO_TYPE_MAPPER_IMPL_H
#define COMMPROTO_TYPE_MAPPER_IMPL_H
#include <TypeMapper.h>
#include <map>

namespace commproto
{
	namespace data
	{
		class TypeMapperImpl : public TypeMapper
		{
		public:
			TypeMapperImpl();
		protected:
			uint32_t registerType(const std::string& type) override;
			bool hasType(const std::string& type) override;
		private:
			std::map<std::string, uint32_t> types;
			uint32_t counter;
		};


	}
}

#endif // COMMPROTO_TYPE_MAPPER_IMPL_H