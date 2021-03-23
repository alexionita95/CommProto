#ifndef COMMPROTO_SERVICE_PARSER_DELEGATOR_FACTORY_H
#define COMMPROTO_SERVICE_PARSER_DELEGATOR_FACTORY_H
#include <commproto/parser/ParserDelegator.h>
#include <commproto/endpoint/ChannelParserDelegator.h>


namespace commproto
{
	namespace service
	{
		class Dispatch;
		class Connection;
		class ParserDelegatorFactory
		{
		public:
			static parser::ParserDelegatorHandle build(Connection& connection, Dispatch * );
		};

	}

	namespace simulator
	{
		class ParserDelegatorFactory
		{
		public:
			static parser::ParserDelegatorHandle build();
		};

	}

	namespace endpoint
	{
		class ParserDelegatorFactory
		{
		public:
			static parser::ParserDelegatorHandle build(const endpoint::ChannelParserDelegatorHandle & channelDelegator);
		};

	}

}
#endif // COMMPROTO_PARSER_DELEGATOR_BUILDER_H