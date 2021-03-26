#ifndef COMMPROTO_SERVICE_CHANNEL_PARSER_DELEGATOR_H
#define COMMPROTO_SERVICE_CHANNEL_PARSER_DELEGATOR_H

#include <map>

#include <commproto/parser/ParserDelegator.h>
#include <commproto/endpoint/DelegatorProvider.h>

namespace commproto
{
	namespace endpoint
	{
		class ChannelParserDelegator : public parser::ParserDelegator
		{
		public:
			ChannelParserDelegator(const DelegatorProviderHandle& provider);
			bool parse(Message & msg) override;
			void notifyMapping(const std::string & name, const uint32_t id);
			void notifyTermination(const uint32_t id);
			void addDelegator(const uint32_t id, const parser::ParserDelegatorHandle & delegator);
		private:
			std::map<uint32_t, parser::ParserDelegatorHandle> delegators;
			DelegatorProviderHandle provider;
		};

		using ChannelParserDelegatorHandle = std::shared_ptr<ChannelParserDelegator>;
	}
}

#endif //COMMPROTO_SERVICE_CHANNEL_PARSER_DELEGATOR_H