#ifndef COMMPROTO_SERVICE_CHAINS_H
#define COMMPROTO_SERVICE_CHAINS_H

#include <commproto/messages/GenericPropertyChain.h>


namespace commproto
{
	namespace service
	{
		using RegisterChannelMessage = messages::SinglePropertyMessage<std::string>;

		using RegisterChannelParser = messages::SinglePropertyParser<std::string>;

		using RegisterChannelSerializer = messages::SinglePropetySerializer<std::string>;

	}
	namespace messages {
		DEFINE_DATA_TYPE(service::RegisterChannelMessage);
	}
}

#endif //COMMPROTO_SERVICE_CHAINS_H