#ifndef ENDPOINT_PARSER_DELEGATOR_FACTORY_H
#define ENDPOINT_PARSER_DELEGATOR_FACTORY_H
#include <commproto/parser/ParserDelegator.h>
#include <commproto/endpoint/ChannelParserDelegator.h>

namespace commproto
{
	namespace endpoint
	{
		class ParserDelegatorFactory
		{
		public:
			static parser::ParserDelegatorHandle build(const ChannelParserDelegatorHandle & channelDelegator);
		};

	}
}

#endif //ENDPOINT_PARSER_DELEGATOR_FACTORY_H