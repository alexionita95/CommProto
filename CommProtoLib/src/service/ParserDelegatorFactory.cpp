#include <commproto/service/ParserDelegatorFactory.h>
#include <commproto/service/ParserDelegator.h>
#include <commproto/parser/ParserDelegatorFactory.h>



namespace commproto {
	namespace service {

		parser::ParserDelegatorHandle ParserDelegatorFactory::build(Connection& connection)
		{
			std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<ParserDelegator>(connection);
			parser::buildBase(delegator);

			return delegator;
		}
	}
}
