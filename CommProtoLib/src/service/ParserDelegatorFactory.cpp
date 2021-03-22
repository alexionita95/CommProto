#include <commproto/service/ParserDelegatorFactory.h>
#include <commproto/service/ParserDelegator.h>
#include <commproto/parser/ParserDelegatorFactory.h>
#include <commproto/service/ServiceChains.h>
#include <commproto/service/ServiceHandlers.h>

namespace commproto {

	template <typename ParserType, typename MessageType>
	void addParserHandlerPair(const parser::ParserDelegatorHandle & delegator, const parser::HandlerHandle &handler)
	{
		parser::ParserHandle parser = std::make_shared<ParserType>(handler);

		delegator->registerParser<MessageType>(parser);
	}

	namespace service {

		parser::ParserDelegatorHandle ParserDelegatorFactory::build(Connection& connection, Dispatch * dispatch)
		{
			std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<ParserDelegator>(connection);
			parser::buildBase(delegator);

			addParserHandlerPair<RegisterChannelParser, RegisterChannelMessage>(delegator, std::make_shared<RegisterChannelHandler>(dispatch,connection.getId()));
			addParserHandlerPair<SubscribeParser, SubscribeMessage>(delegator, std::make_shared<SubscribeHandler>(connection));
			addParserHandlerPair<UnsubscribeParser, UnsubscribeMessage>(delegator, std::make_shared<UnsubscribeHandler>(connection));

			return delegator;
		}
	}

	parser::ParserDelegatorHandle endpoint::ParserDelegatorFactory::build()
	{
		std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<parser::ParserDelegator>();
		parser::buildBase(delegator);

		addParserHandlerPair<service::RegisterIdParser, service::RegisterIdMessage>(delegator,std::make_shared<service::RegisterIdHandler>());
		addParserHandlerPair<service::ChannelMappingParser, service::ChannelMappingMessage>(delegator,std::make_shared<service::MappingHandler>());
	
		return delegator;
	}
}
