#ifndef TOGGLE_H
#define TOGGLE_H
#include <commproto/control/Control.h>
#include <functional>

namespace commproto
{
	namespace control
	{
		using ToggleAction = std::function<void(bool)>;
		namespace endpoint
		{
			class Toggle : public Control {
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name, id } {}
				virtual void toggle() = 0;
			};
			using ToggleHandle = std::shared_ptr<Toggle>;

		}
		namespace ux
		{
			class Toggle : public Control {
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name , id } {}
				virtual void toggle() = 0;
				virtual bool getState() const = 0;
			};
			using ToggleHandle = std::shared_ptr<Toggle>;
		}
	}
}


#endif // TOGGLE_H