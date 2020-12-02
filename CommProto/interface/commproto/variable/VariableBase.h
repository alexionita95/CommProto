#ifndef COMMPROTO_VARIABLE_BASE_H
#define COMMPROTO_VARIABLE_BASE_H

#include <stdint.h>
#include <memory>
#include <commproto/common/Common.h>

namespace commproto
{
	namespace variable
	{

		class Context; 
		using ContextHandle = std::shared_ptr<Context>;

		enum class ValueType : uint8_t
		{
			integer32,
			string,
			real32,
			bool8
		};

		class VariableBase
		{
		public:
			VariableBase(const ValueType type, const ContextHandle &ctx)
				: underlyingType{ type }
				, context{ ctx }
				, index{ 0 }
			{

			}
			void setIndex(const uint32_t index_)
			{
				index = index_;
			}

			ValueType getType() const
			{
				return underlyingType;
			}


            virtual std::string toString() = 0;

			virtual Message serialize() = 0;
			virtual ~VariableBase() = default;

		protected:
			ValueType underlyingType;
			ContextHandle context;
			uint32_t index;
		};

		class VariableBase;
		using VariableBaseHandle = std::shared_ptr<VariableBase>;
	}
}

#endif //COMMPROTO_VARIABLE_BASE_H
