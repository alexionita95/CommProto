#ifndef COMMPROTO_PARSER_DELEGATOR_FACTORY_H
#define COMMPROTO_PARSER_DELEGATOR_FACTORY_H
#include <ParserDelegator.h>

namespace commproto
{
    namespace data
	{
		class ParserDelegatorFactory
		{
		public:
			static ParserDelegatorHandle build();
		};
    }
}
#endif // COMMPROTO_PARSER_DELEGATOR_BUILDER_H