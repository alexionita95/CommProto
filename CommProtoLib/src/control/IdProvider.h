#ifndef CONTROL_ID_PROVIDER_H
#define CONTROL_ID_PROVIDER_H
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/TypeMapper.h>

#include "ButtonImpl.h"
#include "ToggleImpl.h"
#include "LabelImpl.h"
#include "NotificationImpl.h"

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
					, toggleId{ mapper->registerType<ToggleMessage>() }
					, labelId{ mapper->registerType<LabelMessage>() }
					, labelUpdateId{ mapper->registerType<LabelUpdateMessage>() }
					, notificationId{ mapper->registerType<NotificationMessage>() }
					, displayNotificationId{ mapper->registerType<DisplayNotificationMessage>() }
				{

				}
				const uint32_t buttonId;
				const uint32_t toggleId;
				const uint32_t labelId;
				const uint32_t labelUpdateId;
				const uint32_t notificationId;
				const uint32_t displayNotificationId;
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