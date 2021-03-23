#include <commproto/service/ChannelParserDelegator.h>
#include "commproto/logger/Logging.h"


namespace commproto
{
	namespace service
	{

		ChannelParserDelegator::ChannelParserDelegator(const DelegatorProviderHandle& provider_)
			:provider{provider_}
		{
		}

		bool ChannelParserDelegator::parse(Message& msg)
		{
			parser::ByteStream stream(msg);
			uint32_t senderId = 0;
			//read twice to ignore message Id
			stream.read(senderId);
			stream.read(senderId);

			auto it = delegators.find(senderId);
			if (it == delegators.end())
			{
				LOG_WARNING("Could not find a parser delegator for sender id %d", senderId);
				return false;
			}
			it->second->parse(msg);
			return true;
		}

		void ChannelParserDelegator::notifyMapping(const std::string& name, const uint32_t id)
		{
			if (delegators.find(id) != delegators.end())
			{
				return;
			}

			addDelegator(id, provider->provide(name));
		}

		void ChannelParserDelegator::addDelegator(const uint32_t id, const parser::ParserDelegatorHandle& delegator)
		{
			if (delegators.find(id) != delegators.end())
			{
				return;
			}
			delegators.insert({id,delegator});
		}

	}
}
