#ifndef UX_TOGGLE_H
#define UX_TOGGLE_H
#include <commproto/control/ux/Control.h>
#include <functional>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class Toggle : public Control
			{
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name , id } {}
				virtual void toggle() = 0;
				virtual bool getState() const = 0;
			};
			using ToggleHandle = std::shared_ptr<Toggle>;
		}
	}
}


#endif // UX_TOGGLE_H