#ifndef UX_NOTIFICATION_H
#define UX_NOTIFICATION_H
#include <commproto/control/ux/Control.h>
#include <functional>

namespace commproto
{
	namespace control
	{
		namespace ux
		{

			class Notification : public Control
			{
			public:
				Notification(const std::string& name, uint32_t id)
					: Control(name, id)
				{
				}
				virtual void execute(const std::string & option) = 0;
				virtual std::vector<std::string> getOptions() const = 0;
			};

			using NotificationHandle = std::shared_ptr<Notification>;
		}
	}
}

#endif //UX_NOTIFICATION_H