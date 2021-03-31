#ifndef CONTROL_ID_PROVIDER_H
#define CONTROL_ID_PROVIDER_H

#include "../parser/TypeMapperImpl.h"
#include "ButtonImpl.h"

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
				{

				}
				const uint32_t buttonId;
			};
		}

	}
}

#endif // CONTROL_ID_PROVIDER_H