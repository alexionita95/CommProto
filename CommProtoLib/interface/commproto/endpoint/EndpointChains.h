#ifndef ENDPOINT_CHAINS_H
#define ENDPOINT_CHAINS_H

#include <commproto/messages/SinglePropertyChain.h>
#include <commproto/messages/DoublePropertyChain.h>


namespace commproto
{

	namespace endpoint
	{
		//register Id
		MAKE_SINGLE_PROP_MESSAGE(RegisterIdMessage, uint32_t);
		using RegisterIdParser = messages::SinglePropertyParser<uint32_t>;
		using RegisterIdSerializer = messages::SinglePropertySerializer<uint32_t>;

		//channel mapping
		MAKE_DOUBLE_PROP_MESSAGE(ChannelMappingMessage, std::string, uint32_t);
		using ChannelMappingParser = messages::DoublePropertyParser<std::string, uint32_t>;
		using ChannelMappingSerializer = messages::DoublePropertySerializer<std::string, uint32_t>;

		//channel termination
		MAKE_SINGLE_PROP_MESSAGE(ChannelTerminationMessage, uint32_t);
		using ChannelTerminationParser = messages::SinglePropertyParser<uint32_t>;
		using ChannelTerminationSerializer = messages::SinglePropertySerializer<uint32_t>;

	}

	DEFINE_DATA_TYPE(endpoint::RegisterIdMessage);
	DEFINE_DATA_TYPE(endpoint::ChannelMappingMessage);
	DEFINE_DATA_TYPE(endpoint::ChannelTerminationMessage);
	
}

#endif //ENDPOINT_CHAINS_H