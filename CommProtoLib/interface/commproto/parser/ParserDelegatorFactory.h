#ifndef COMMPROTO_PARSER_DELEGATOR_FACTORY_H
#define COMMPROTO_PARSER_DELEGATOR_FACTORY_H
#include <commproto/parser/ParserDelegator.h>
#include <commproto/variable/Context.h>

namespace commproto
{
    namespace parser
	{
		void buildBase(const ParserDelegatorHandle & delegator);

		class ParserDelegatorFactory
		{
		public:
			static ParserDelegatorHandle build(const variable::ContextHandle& context);
		};
    }

}
#endif // COMMPROTO_PARSER_DELEGATOR_BUILDER_H