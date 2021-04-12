#ifndef LABEL_CHAINS_H
#define LABEL_CHAINS_H

#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/messages/DoublePropertyChain.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			MAKE_TRIPLE_PROP_MESSAGE(LabelMessage, uint32_t, std::string, std::string);

			using LabelParser = messages::TriplePropertyParser<uint32_t, std::string, std::string>;
			using LabelSerializer = messages::TriplePropertySerializer<uint32_t, std::string, std::string>;

			MAKE_DOUBLE_PROP_MESSAGE(LabelUpdateMessage, uint32_t, std::string);

			using LabelUpdateParser = messages::DoublePropertyParser<uint32_t, std::string>;
			using LabelUpdateSerializer = messages::DoublePropertySerializer<uint32_t, std::string>;
		}

	}
	DEFINE_DATA_TYPE(control::endpoint::LabelMessage);
	DEFINE_DATA_TYPE(control::endpoint::LabelUpdateMessage);
}

#endif// LABEL_CHAINS_H
