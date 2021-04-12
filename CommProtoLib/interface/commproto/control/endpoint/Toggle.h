#ifndef ENDPOINT_TOGGLE_H
#define ENDPOINT_TOGGLE_H
#include <commproto/control/endpoint/Control.h>
#include <functional>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			using ToggleAction = std::function<void(bool)>;
			class Toggle : public Control {
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name, id } {}
				virtual void toggle() = 0;
			};
			using ToggleHandle = std::shared_ptr<Toggle>;

		}
	}
}


#endif // ENDPOINT_TOGGLE_H