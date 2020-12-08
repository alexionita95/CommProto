#ifndef COMMPROTO_VARIABLE_HANDLER_H
#define COMMPROTO_VARIABLE_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/variable/Context.h>
#include <commproto/variable/VariableMessage.h>
#include <commproto/variable/Variable.h>

namespace commproto
{
	namespace variable
	{
		class VariableHandler : public parser::Handler
		{
		public:
			VariableHandler(const ContextHandle &ctx)
				: context{ ctx }
			{

			}

			template <typename T, ValueType U>
			void handle(VariableMessageTemplated<T>& msg);
			void handle(messages::MessageBase&& data) override;

		private:
			ContextHandle context;
		};

		inline void VariableHandler::handle(messages::MessageBase&& data)
		{
			
			auto& var = static_cast<VariableMessage&>(data);
			LOG_DEBUG("Handling a variable with index %d",var.index);
			switch (var.valueType)
			{
			case ValueType::integer32:
			{
				LOG_DEBUG("Index %d is an integer", var.index);
				handle<uint32_t, ValueType::integer32>(static_cast<VariableMessageTemplated<uint32_t>&>(var));
			}
			break;
			case ValueType::string:
			{
				LOG_DEBUG("Index %d is a string", var.index);
				handle<std::string, ValueType::string>(static_cast<VariableMessageTemplated<std::string>&>(var));
			}
			break;
			case ValueType::real32:
			{
				LOG_DEBUG("Index %d is a real value", var.index);
				handle<float, ValueType::real32>(static_cast<VariableMessageTemplated<float>&>(var));
			}
			break;
			case ValueType::bool8:
				LOG_DEBUG("Index %d is a boolean", var.index);
				handle<bool, ValueType::bool8>(static_cast<VariableMessageTemplated<bool>&>(var));
			break;
			}
		}

		template <typename T, ValueType U>
		void VariableHandler::handle(VariableMessageTemplated<T>& msg)
		{

			LOG_DEBUG("Internal handling a variable with index %d", msg.index);
			VariableBaseHandle var = context->get(msg.index); // try to get the variable from the context
			if (!var)
			{
				//no variable was found, so we make a new one and pass it to the context
				std::shared_ptr<Variable<T, U>> variable = std::make_shared<Variable<T, U>>(context, msg.value);
				context->registerInVariable(variable);
				return;
			}

			//we found a variable, try to cast it to what we know it is and set its value
			std::shared_ptr<Variable<T, U>> variable = std::static_pointer_cast<Variable<T, U>>(var);
			if (!variable)
			{
				return;
			}
			variable->set(msg.value);
		}
	}
}


#endif //COMMPROTO_VARIABLE_HANDLER_H