#include <commproto/parser/ParserDelegatorFactory.h>
#include  "MappingType.h"
#include <commproto/variable/Variable.h>
#include <commproto/variable/VariableParser.h>
#include <commproto/variable/VariableHandler.h>
#include <commproto/variable/VariableMappingParser.h>
#include <commproto/variable/VariableMappingHandler.h>

namespace commproto
{
	namespace parser
	{

		class MappingTypeParser : public Parser
		{
		public:
			MappingTypeParser(const HandlerHandle & handler) : Parser{ handler } {}
			void parse(ByteStream&& reader) override
			{
				std::string name;
				uint32_t typeId;
				uint32_t sender = 0;
				reader.read(sender);
				reader.read(name);
				reader.read(typeId);
				handler->handle(std::move(messages::MappingType(name, typeId)));
			}
		};


		class MappingTypeHandler : public Handler
		{
		public:
			MappingTypeHandler(const ParserDelegatorHandle & delegator_)
				: delegator{ delegator_ }
			{

			}

			void handle(messages::MessageBase && data) override
			{
				messages::MappingType & mapping = static_cast<messages::MappingType&>(data);
				delegator->registerMapping(mapping.name, mapping.typeId);
			}

		private:
			ParserDelegatorHandle delegator;
		};


		void buildBase(const ParserDelegatorHandle & delegator)
		{

			ParserHandle mappingParser = std::make_shared<MappingTypeParser>(std::make_shared<MappingTypeHandler>(delegator));

			delegator->registerParser<messages::MappingType>(mappingParser);
			delegator->registerMapping(messages::MessageName<messages::MappingType>::name(), 0);
		}

		ParserDelegatorHandle ParserDelegatorFactory::build(const variable::ContextHandle& context)
		{

			ParserDelegatorHandle delegator = std::make_shared<ParserDelegator>();
			buildBase(delegator);

			ParserHandle variableParser = std::make_shared<variable::VariableParser>(std::make_shared<variable::VariableHandler>(context));

			ParserHandle variableMappingParser = std::make_shared<variable::VariableMappingParser>(std::make_shared<variable::VariableMappingHandler>(context));

			delegator->registerParser<variable::VariableMessage>(variableParser);
			delegator->registerParser<variable::VariableMappingMessage>(variableMappingParser);
			return delegator;

		}

	}


}
