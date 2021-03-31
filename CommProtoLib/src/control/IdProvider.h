#ifndef CONTROL_ID_PROVIDER_H
#define CONTROL_ID_PROVIDER_H

#include "../parser/TypeMapperImpl.h"
#include "ButtonImpl.h"
#include <commproto/service/ServiceChains.h>

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
					: buttonId{ mapper->registerType<ButtonMessage>() }
				{

				}
				const uint32_t buttonId;
			};

		}

		namespace ux
		{
			class IdProvider
			{
			public:
				IdProvider(const messages::TypeMapperHandle& mapper)
					: buttonId{ mapper->registerType<PressButtonMessage>() }
					, sendToId(mapper->registerType<service::SendToMessage>())
				{

				}
				const uint32_t buttonId;
				const uint32_t sendToId;
			};
		}

	}
}

#endif // CONTROL_ID_PROVIDER_H