#ifndef BUTTON_CHAINS_H
#define BUTTON_CHAINS_H

#include <commproto/messages/DoublePropertyChain.h>
#include <commproto/messages/SinglePropertyChain.h>

namespace commproto
{
    namespace control
    {
		namespace endpoint
		{
			MAKE_DOUBLE_PROP_MESSAGE(ButtonMessage, uint32_t, std::string);

			using ButtonParser = messages::DoublePropertyParser< uint32_t, std::string>;
			using ButtonSerializer = messages::DoublePropertySerializer<uint32_t, std::string>;

		}

		namespace ux
		{
			MAKE_SINGLE_PROP_MESSAGE(PressButtonMessage, uint32_t);

			using PressButtonParser = messages::SinglePropertyParser<uint32_t>;
			using PressButtonSerializer = messages::SinglePropertySerializer<uint32_t>;
		}
    }

	DEFINE_DATA_TYPE(control::endpoint::ButtonMessage);
	DEFINE_DATA_TYPE(control::ux::PressButtonMessage);
}

#endif// BUTTON_CHAINS_H