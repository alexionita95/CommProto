#ifndef COMMMPROTO_MAPPING_TYPE_H
#define COMMMPROTO_MAPPING_TYPE_H
#include <TypeBase.h>
#include <TypeName.h>
namespace  commproto
{
	namespace data
	{
		class MappingType : public TypeBase
		{
		public:
			const std::string name;
			const uint32_t typeId;

			MappingType(const std::string& name_, const uint32_t id_)
				: TypeBase{ 0 } //0 is reserved for mapping types
				, name{ name_ }
				, typeId{ id_ } {}


			BP_TYPE_DEFAULTS(MappingType);

			uint32_t getSize() override {
				return sizeOf(name)
					+ sizeof(typeId)
					+ TypeBase::getSize();
			}
		};
	}
};

#endif //COMMMPROTO_MAPPING_TYPE_H