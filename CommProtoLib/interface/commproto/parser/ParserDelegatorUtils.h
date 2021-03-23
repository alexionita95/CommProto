#ifndef PARSER_DELEGATOR_UTILS_H
#define PARSER_DELEGATOR_UTILS_H

#include <commproto/parser/ParserDelegator.h>

namespace commproto
{
    namespace parser
    {
        template <typename ParserType, typename MessageType>
		void addParserHandlerPair(const parser::ParserDelegatorHandle & delegator, const parser::HandlerHandle &handler)
		{
			parser::ParserHandle parser = std::make_shared<ParserType>(handler);

			delegator->registerParser<MessageType>(parser);
		}

		void buildBase(const ParserDelegatorHandle & delegator);
    }
}

#endif //PARSER_DELEGATOR_UTILS_H