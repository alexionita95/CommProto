#ifndef CHANNEL_TERMINATION_HANDLER_H
#define CHANNEL_TERMINATION_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/endpoint/ChannelParserDelegator.h>

namespace commproto
{
namespace endpoint
{
    	class ChannelTerminationHandler : public parser::Handler
		{
		public:
			ChannelTerminationHandler(const ChannelParserDelegatorHandle & delegator);
			void handle(messages::MessageBase&& data) override;
		private:
			ChannelParserDelegatorHandle delegator;
		};
}
}

#endif// CHANNEL_TERMINATION_HANDLER_H