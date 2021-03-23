#include <commproto/endpoint/ParserDelegatorFactory.h>
#include <commproto/service/ParserDelegator.h>
#include <commproto/endpoint/ChannelMappingHandler.h>
#include <commproto/endpoint/EndpointChains.h>
#include <commproto/endpoint/RegisterIdHandler.h>
#include <commproto/parser/ParserDelegatorUtils.h>

namespace commproto {

	parser::ParserDelegatorHandle endpoint::ParserDelegatorFactory::build(const endpoint::ChannelParserDelegatorHandle & channelDelegator)
	{
		std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<parser::ParserDelegator>();
		parser::buildBase(delegator);

		parser::addParserHandlerPair<RegisterIdParser, RegisterIdMessage>(delegator,std::make_shared<RegisterIdHandler>());
		parser::addParserHandlerPair<ChannelMappingParser, ChannelMappingMessage>(delegator,std::make_shared<ChannelMappingHandler>(channelDelegator));
	
		return delegator;
	}

}
