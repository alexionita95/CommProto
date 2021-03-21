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

	}
	namespace messages {
		DEFINE_DATA_TYPE(service::RegisterChannelMessage);
		DEFINE_DATA_TYPE(service::RegisterIdMessage);
	}
}

#endif //COMMPROTO_SERVICE_CHAINS_H