#ifndef COMMPROTO_VALUE_H
#define COMMPROTO_VALUE_H

#include <commproto/parser/Parser.h>
#include <commproto/messages/MessageBase.h>

namespace commproto
{
	namespace variable
	{

		class VariableMessage : public messages::MessageBase
		{
		public:
			ValueType valueType; //used to cast to the correct sub-type
			uint32_t index;

			VariableMessage(const uint32_t type, const ValueType valueType_, const uint32_t index)
				: MessageBase{ type }
				, valueType{ valueType_ }
				, index{ index }
			{}

			BP_TYPE_DEFAULTS(VariableMessage);

			uint32_t getSize() const override
			{
				return messages::MessageBase::getSize()
					+ sizeof(valueType)
					+ sizeof(index);
			}
		};
	

		template <typename T>
		class VariableMessageTemplated : public VariableMessage
		{
		public:
			T value;
			VariableMessageTemplated(const uint32_t type, const ValueType valueType_, const uint32_t index, T value_)
				: VariableMessage{ type , valueType_ , index }
				, value{ value_ }
			{

			}

			BP_TYPE_DEFAULTS(VariableMessageTemplated);


			uint32_t getSize() const override
			{
				return VariableMessage::getSize() + messages::sizeOf(value);
			}
		};



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
		void print(const T & val){
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

		class VariableHandler;


		template <typename T, ValueType  UnderlyingType>
		class Variable : public VariableBase
		{
		public:
			Variable(const ContextHandle &ctx, const T & value_ = T{})
				: VariableBase{ UnderlyingType, ctx }
				, value{ value_ }
			{

			}

			//used to set the value by the owner, notifying the context that something has changed
			Variable& operator=(const T& value_)
			{
				value = value_;
				context->notifyOut(serialize());
				return *this;
			}

			void printValue() override
			{
				print(value);
			}
			Message serialize() override
			{
				printf("Sending a message to notift a variable update : [index:%d]\n", index);
				return serializer.serialize<T>(std::move(VariableMessageTemplated<T>(context->getMessageType(), UnderlyingType, index, value)));
			}

			~Variable() = default;

			const T& get() const
			{
				return value;
			}
		private:

			//used to set the value externally by a handler, notifying the context that an update of the value occured
			void set(const T& value_)
			{
				value = value_;
				context->notifyIn(index);
			}
			T value;
			VariableSerializer serializer;
			friend class VariableHandler;
		};

		using IntegerVariable = Variable<uint32_t, ValueType::integer32>;
		using IntegerVariableHandle = std::shared_ptr<IntegerVariable>;

		using StringVariable = Variable<std::string, ValueType::string>;
		using StringVariableHandle = std::shared_ptr<StringVariable>;

		using RealVariable = Variable<float, ValueType::real32>;
		using RealVariableHandle = std::shared_ptr<RealVariable>;


		class VariableParser : public parser::Parser
		{
		public:

			VariableParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			template <typename T>
			void parse(parser::ByteStream && msg, const uint32_t index, const ValueType type)
			{
				T value{};
				msg.read(value);
				printf("Read a value, sending to handler ...\n");
				handler->handle(std::move(VariableMessageTemplated<T>(0, type, index, value)));
			}

			void parse(parser::ByteStream && msg) override
			{
				uint8_t valueType = 0;
				uint32_t index = 0;
				msg.read(valueType);
				msg.read(index);
				ValueType type = static_cast<ValueType>(valueType);

				printf("Handling a variable with type %d...\n", type);
				switch (type)
				{
				case ValueType::integer32:
					parse<uint32_t>(std::move(msg), index, type);
					break;
				case ValueType::string:
					parse<std::string>(std::move(msg), index, type);
					break;
				case ValueType::real32:
					parse<float>(std::move(msg), index, type);
					break;
				default:;
				}


			}
		};

		class VariableHandler : public parser::Handler
		{
		public:
			VariableHandler(const ContextHandle &ctx)
				:context{ ctx }
			{

			}

			template <typename T, ValueType U>
			void handle(VariableMessageTemplated<T> & msg)
			{
				VariableBaseHandle var = context->get(msg.index);
				printf("Handling a generic variable...\n");
				if (!var)
				{
					//TODO: Add mechanism for sending the fact that there is a new in variable
					//if the context does not have a variable that corresponds to what we got, then we can't do anything with it

					std::shared_ptr<Variable<T, U>> variable = std::make_shared<Variable<T, U>>(context, msg.value);
					uint32_t index = context->registerInVariable(variable);
					printf("Registered a variable with index = %d\n",index);
					variable->setIndex(index);
					variable->printValue();
					return;
				}

				
				std::shared_ptr<Variable<T, U>> variable = std::static_pointer_cast<Variable<T, U>>(var);
				variable->printValue();
				if(!variable)
				{
					printf("An issue occurred when trying to cast variable with index %d\n", variable->index);
				}
				printf("Variable found in context with index %d, setting new value...\n", variable->index);
				variable->set(msg.value);
			}

			void handle(messages::MessageBase && data) override
			{
				auto& var = static_cast<VariableMessage&>(data);

				printf("Handling a variable with type %d...\n", var.valueType);
				switch (var.valueType)
				{
				case ValueType::integer32:
				{
					handle<uint32_t, ValueType::integer32>(static_cast<VariableMessageTemplated<uint32_t>&>(var));
				}
				break;
				case ValueType::string:
				{
					handle<std::string, ValueType::string>(static_cast<VariableMessageTemplated<std::string>&>(var));
				}
				break;
				case ValueType::real32: 
				{
					handle<float, ValueType::real32>(static_cast<VariableMessageTemplated<float>&>(var));
				}
				break;
				}
			}
		private:
			ContextHandle context;
		};
	}

	namespace messages
	{
		DEFINE_DATA_TYPE(variable::VariableMessage);
	}
}


#endif