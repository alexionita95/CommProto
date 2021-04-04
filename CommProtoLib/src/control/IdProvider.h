#ifndef CONTROL_ID_PROVIDER_H
#define CONTROL_ID_PROVIDER_H
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/MessageMapper.h>

#include "ButtonImpl.h"
#include "ToggleImpl.h"

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			class IdProvider
			{
			public:
				IdProvider(const messages::TypeMapperHandle& mapper)
					: buttonId{ mapper->registerType<ButtonMessage>()}
					, toggleId{ mapper->registerType<ToggleMessage>()}
				{

				}
				const uint32_t buttonId;
				const uint32_t toggleId;
			};

		}

		namespace ux
		{
			class IdProvider
			{
			public:
				IdProvider(const messages::TypeMapperHandle& mapper)
					: buttonId{ mapper->registerType<PressButtonMessage>() }
					, toggleId{ mapper->registerType<ToggleMessage>() }
					, sendToId(mapper->registerType<service::SendToMessage>())
				{

				}
				const uint32_t buttonId;
				const uint32_t toggleId;
				const uint32_t sendToId;
			};
		}

	}
}

#endif // CONTROL_ID_PROVIDER_H