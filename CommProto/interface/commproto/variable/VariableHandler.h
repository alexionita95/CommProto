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

		template <typename T, ValueType U>
		void VariableHandler::handle(VariableMessageTemplated<T>& msg)
		{
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