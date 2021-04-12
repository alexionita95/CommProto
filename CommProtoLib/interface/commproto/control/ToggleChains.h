#ifndef TOGGLE_CHAINS_H
#define TOGGLE_CHAINS_H
#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/messages/SinglePropertyChain.h>

namespace commproto
{
    namespace control
    {

		namespace endpoint
		{
			MAKE_TRIPLE_PROP_MESSAGE(ToggleMessage, uint32_t, std::string, bool);

			using ToggleParser = messages::TriplePropertyParser<uint32_t, std::string, bool>;
			using ToggleSerializer = messages::TriplePropertySerializer<uint32_t, std::string, bool>;
		}

		namespace ux
		{

			MAKE_SINGLE_PROP_MESSAGE(ToggleMessage, uint32_t);
			using ToggleParser = messages::SinglePropertyParser<uint32_t>;
			using ToggleSerializer = messages::SinglePropertySerializer<uint32_t>;
		}
    }
	DEFINE_DATA_TYPE(control::endpoint::ToggleMessage);
	DEFINE_DATA_TYPE(control::ux::ToggleMessage);
}


#endif// TOGGLE_CHAINS_H