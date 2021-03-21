#ifndef COMMPROTO_SERVICE_CHAINS_H
#define COMMPROTO_SERVICE_CHAINS_H

#include <commproto/messages/GenericPropertyChain.h>


namespace commproto
{
	namespace service
	{

		//register channel
		MAKE_SINGLE_PROP_MESSAGE(RegisterChannelMessage,std::string);

		using RegisterChannelParser = messages::SinglePropertyParser<std::string>;

		using RegisterChannelSerializer = messages::SinglePropetySerializer<std::string>;

		//register Id
		MAKE_SINGLE_PROP_MESSAGE(RegisterIdMessage, uint32_t);
		
		using RegisterIdParser = messages::SinglePropertyParser<uint32_t>;

		using RegisterIdSerializer = messages::SinglePropetySerializer<uint32_t>;

		//subscribe
		MAKE_SINGLE_PROP_MESSAGE(SubscribeMessage, std::string);

		using SubscribeParser = messages::SinglePropertyParser<std::string>;

		using SubscribeSerializer = messages::SinglePropetySerializer<std::string>;

		//channel mapping
		MAKE_DOUBLE_PROP_MESSAGE(ChannelMappingMessage, std::string, uint32_t);

		using ChannelMappingParser = messages::DoublePropertyParser<std::string, uint32_t>;

		using ChannelMappingSerializer = messages::DoublePropertySerializer<std::string, uint32_t>;


		//unsubscribe
		MAKE_SINGLE_PROP_MESSAGE(UnsubscribeMessage, std::string);

		using UnsubscribeParser = messages::SinglePropertyParser<std::string>;

		using UnsubscribeSerializer = messages::SinglePropetySerializer<std::string>;

	}
	namespace messages {
		DEFINE_DATA_TYPE(service::RegisterChannelMessage);
		DEFINE_DATA_TYPE(service::RegisterIdMessage);
		DEFINE_DATA_TYPE(service::SubscribeMessage);
		DEFINE_DATA_TYPE(service::UnsubscribeMessage);
		DEFINE_DATA_TYPE(service::ChannelMappingMessage);
	}
}

#endif //COMMPROTO_SERVICE_CHAINS_H