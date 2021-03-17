#include <commproto/service/ParserDelegatorFactory.h>
#include <commproto/service/ParserDelegator.h>
#include <commproto/parser/ParserDelegatorFactory.h>
#include <commproto/service/RegisterChannelChain.h>

namespace commproto {
	namespace service {

		parser::ParserDelegatorHandle ParserDelegatorFactory::build(Connection& connection, Dispatch * dispatch)
		{
			std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<ParserDelegator>(connection);
			parser::buildBase(delegator);

			parser::HandlerHandle registerChannelHandler = std::make_shared<RegisterChannelHandler>(dispatch,connection.getId());
			parser::ParserHandle registerChannelParser = std::make_shared<RegisterChannelParser>(registerChannelHandler);

			delegator->registerParser<RegisterChannelMessage>(registerChannelParser);

			return delegator;
		}
	}
}
