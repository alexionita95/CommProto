#ifndef COMMPROTO_TYPEBASE_H
#define  COMMPROTO_TYPEBASE_H
#include <stdint.h>
#include <string>
#include <vector>


//used to define boilerplate rule of 5 since we want to use std::move with types derived from TypeBase
#define BP_TYPE_DEFAULTS(Class)						\
	Class(const Class& other) = default;			\
	Class(Class&& other) = default;					\
	Class& operator=(const Class& other) = default;	\
	Class& operator=(Class&&other) = default;		\
	virtual ~Class() = default;						\


namespace commproto
{
	namespace data
	{
		template <typename T>
		uint32_t sizeOf(const T& value)
		{
			return sizeof(value);
		}

		template <>
		uint32_t sizeOf(const std::string & value)
		{
			uint32_t size = sizeof(uint32_t) //first we write the size of the string
				+ value.size(); //then the amount of chars
		}


		template <typename T>
		uint32_t sizeOf(const std::vector<T>& values)
		{
			uint32_t size = sizeof(uint32_t); //first we write the size of the vector
			for (const auto & elem : values)
			{
				size += sizeOf(elem); //then add size of each element
			}
			return size;
		}

		class TypeBase
		{
		public:
			TypeBase(const uint32_t type_) : type{ type_ } {};

			BP_TYPE_DEFAULTS(TypeBase);

			const uint32_t type;
			virtual uint32_t getSize()
			{
				return sizeof(type);
			}

		};
	}
}

#endif //COMMPROTO_TYPEBASE_H