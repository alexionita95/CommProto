#ifndef COMMPROTO_VARIABLE_MAPPING_HANDLER_H
#define COMMPROTO_VARIABLE_MAPPING_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/variable/Context.h>
#include <commproto/variable/VariableMappingMessage.h>

namespace commproto
{
	namespace variable
	{
		class VariableMappingHandler : public parser::Handler
		{
		public:
			VariableMappingHandler(const ContextHandle &ctx)
				: context{ ctx }
			{

			}

			void handle(messages::MessageBase&& data) override;

		private:
			ContextHandle context;
		};

		inline void VariableMappingHandler::handle(messages::MessageBase&& data)
		{
			auto& var = static_cast<VariableMappingMessage&>(data);
			context->registerMapping(var.name, var.index);
			
		}

	}
}


#endif //COMMPROTO_VARIABLE_HANDLER_H