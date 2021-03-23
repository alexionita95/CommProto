#ifndef CHANNEL_MAPPING_HANDLER_H
#define CHANNEL_MAPPING_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/endpoint/ChannelParserDelegator.h>

namespace commproto
{
namespace endpoint
{
    	class ChannelMappingHandler : public parser::Handler
		{
		public:
			ChannelMappingHandler(const ChannelParserDelegatorHandle & delegator);
			void handle(messages::MessageBase&& data) override;
		private:
			ChannelParserDelegatorHandle delegator;
		};
}
}

#endif// CHANNEL_MAPPING_HANDLER_H