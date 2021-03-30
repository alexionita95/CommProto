#include <commproto/service/ParserDelegatorFactory.h>
#include <commproto/service/ParserDelegator.h>
#include <commproto/service/ServiceChains.h>
#include <commproto/service/SubscribeHandler.h>
#include <commproto/service/UnsubscribeHandler.h>
#include <commproto/service/RegisterChannelHandler.h>
#include <commproto/parser/ParserDelegatorUtils.h>
#include <commproto/service/SendToHandler.h>

namespace commproto {

	namespace service {

		parser::ParserDelegatorHandle ParserDelegatorFactory::build(Connection& connection, Dispatch * dispatch)
		{
			std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<ParserDelegator>(connection);
			parser::buildBase(delegator);

			parser::addParserHandlerPair<RegisterChannelParser, RegisterChannelMessage>(delegator, std::make_shared<RegisterChannelHandler>(dispatch,connection.getId()));
			parser::addParserHandlerPair<SubscribeParser, SubscribeMessage>(delegator, std::make_shared<SubscribeHandler>(connection));
			parser::addParserHandlerPair<UnsubscribeParser, UnsubscribeMessage>(delegator, std::make_shared<UnsubscribeHandler>(connection));
			parser::addParserHandlerPair<SendtoParser, SendToMessage>(delegator, std::make_shared<SendToHandler>(dispatch));

			return delegator;
		}
	}

}
