#ifndef PARSER_DELEGATOR_UTILS_H
#define PARSER_DELEGATOR_UTILS_H

#include <commproto/parser/ParserDelegator.h>
#include <commproto/sockets/Socket.h>

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


		void sendMappings(const ParserDelegatorHandle & delegator, const sockets::SocketHandle & socket);
    }
}

#endif //PARSER_DELEGATOR_UTILS_H