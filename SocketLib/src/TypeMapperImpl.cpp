#include "TypeMapperImpl.h"

namespace commproto
{
	namespace data
	{
		TypeMapperImpl::TypeMapperImpl()
			: counter(0)
		{
		}

		uint32_t TypeMapperImpl::registerType(const std::string& type)
		{
			auto inMap = types.find(type);

			if (inMap != types.end()) {
				return inMap->second;
			}

			uint32_t id = counter++;
			types.emplace(type, id);
			return id;
		}

		bool TypeMapperImpl::hasType(const std::string& type)
		{
			return types.find(type) != types.end();
		}
	}
}