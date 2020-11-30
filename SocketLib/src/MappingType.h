#ifndef COMMMPROTO_MAPPING_TYPE_H
#define COMMMPROTO_MAPPING_TYPE_H
#include <TypeBase.h>
#include <TypeName.h>
#include <Common.h>
#include "ByteStream.h"

namespace  commproto
{
	namespace data
	{
		class MappingType : public TypeBase
		{
		public:
			std::string name;
			uint32_t typeId;

			MappingType(const std::string& name_, const uint32_t id_)
				: TypeBase{ 0 } //0 is reserved for mapping types
				, name{ name_ }
				, typeId{ id_ } {}


			BP_TYPE_DEFAULTS(MappingType);

			uint32_t getSize() const override {
				return sizeOf(name)
					+ sizeof(typeId)
					+ TypeBase::getSize();
			}
		};

		DEFINE_DATA_TYPE(MappingType);


		class MappingTypeSerializer
		{
		public:
			Message serialize(TypeBase && data)
			{
				MappingType& mapping = static_cast<MappingType&>(data);
				socketlib::ByteStream stream;
				stream.writeHeader(data);
				stream.write(mapping.name);
				stream.write(mapping.typeId);
				return stream.getStream();
			}
		};

	}
};

#endif //COMMMPROTO_MAPPING_TYPE_H