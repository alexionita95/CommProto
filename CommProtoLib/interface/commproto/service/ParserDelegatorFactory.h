#ifndef COMMPROTO_SERVICE_PARSER_DELEGATOR_FACTORY_H
#define COMMPROTO_SERVICE_PARSER_DELEGATOR_FACTORY_H

#include <commproto/parser/ParserDelegator.h>

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

}
#endif // COMMPROTO_PARSER_DELEGATOR_BUILDER_H