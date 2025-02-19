#ifndef UX_CONTROL_ID_PROVIDER_H
#define UX_CONTROL_ID_PROVIDER_H
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/TypeMapper.h>
#include <commproto/control/ButtonChains.h>
#include <commproto/control/ToggleChains.h>
#include <commproto/control/NotificationChains.h>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class IdProvider
			{
			public:
				IdProvider(const messages::TypeMapperHandle& mapper)
					: buttonId{ mapper->registerType<PressButtonMessage>() }
					, toggleId{ mapper->registerType<ToggleMessage>() }
					, sendToId{ mapper->registerType<service::SendToMessage>() }
					, notificationResponseId{ mapper->registerType<NotificationResponseMessage>() }
				{

				}
				const uint32_t buttonId;
				const uint32_t toggleId;
				const uint32_t sendToId;
				const uint32_t notificationResponseId;
			};
		}

	}
}

#endif // CONTROL_ID_PROVIDER_H