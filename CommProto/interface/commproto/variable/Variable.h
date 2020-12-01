#ifndef COMMPROTO_VALUE_H
#define COMMPROTO_VALUE_H

#include <commproto/messages/MessageBase.h>
#include <commproto/variable/VariableBase.h>
#include <commproto/variable/Context.h>
#include <commproto/variable/VariableMessage.h>

namespace commproto
{
	namespace variable
	{
		class VariableSerializer
		{

		public:
			template <typename T>
			Message serialize(messages::MessageBase && data) const
			{
				static const char* const types[] = { "integer", "string", "real" };
				VariableMessageTemplated<T>& value = static_cast<VariableMessageTemplated<T>&>(data);
				parser::ByteStream stream;
				stream.writeHeader(data);
				stream.write(value.valueType);
				stream.write(value.index);
				stream.write(value.value);
				printf("Serialized a message with id:%d type:%s size:%d(valuesize:%d).\n", data.type, types[static_cast<uint8_t>(value.valueType)], data.getSize(), sizeOf(value));
				return stream.getStream();

			}
		};

		template <typename T>
		void print(const T & val) {
		}

		template <>
		inline void print(const uint32_t & val)
		{
			printf("Value = %d\n", val);
		}

		template <>
		inline void print(const std::string & val)
		{
			printf("Value = %s\n", val.c_str());
		}

		template <typename T, ValueType  UnderlyingType>
		class Variable : public VariableBase
		{
		public:
			Variable(const ContextHandle& ctx, const T& value_ = T{});

			//used to set the value by the owner, notifying the context that something has changed
			Variable& operator=(const T& value_);

			void printValue() override;

			Message serialize() override;

			~Variable() = default;

			const T& get() const
			{
				return value;
			}

			//used to set the value externally by a handler, notifying the context that an update of the value occured
			void set(const T& value_)
			{
				value = value_;
				context->notifyIn(index);
			}

		private:

			T value;
			VariableSerializer serializer;
		};

		template <typename T, ValueType UnderlyingType>
		Variable<T, UnderlyingType>::Variable(const ContextHandle& ctx, const T& value_)
			: VariableBase{ UnderlyingType, ctx }
			, value{ value_ }
		{
		}

		template <typename T, ValueType UnderlyingType>
		Variable<T, UnderlyingType>& Variable<T, UnderlyingType>::operator=(const T& value_)
		{
			value = value_;
			context->notifyOut(index);
			return *this;
		}

		template <typename T, ValueType UnderlyingType>
		void Variable<T, UnderlyingType>::printValue()
		{
			print(value);
		}

		template <typename T, ValueType UnderlyingType>
		Message Variable<T, UnderlyingType>::serialize()
		{
			printf("Sending a message to notift a variable update : [index:%d]\n", index);
			return serializer.serialize < T >(std::move(VariableMessageTemplated<T>(context->getMessageType(), UnderlyingType, index, value)));
		}

		using IntegerVariable = Variable<uint32_t, ValueType::integer32>;
		using IntegerVariableHandle = std::shared_ptr<IntegerVariable>;

		using StringVariable = Variable<std::string, ValueType::string>;
		using StringVariableHandle = std::shared_ptr<StringVariable>;

		using RealVariable = Variable<float, ValueType::real32>;
		using RealVariableHandle = std::shared_ptr<RealVariable>;
		
	}
}


#endif