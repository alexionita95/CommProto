#ifndef BUTTON_H
#define BUTTON_H
#include <commproto/control/Control.h>
#include <functional>

namespace commproto
{
	namespace control {
		using ButtonAction = std::function<void()>;
		namespace  endpoint {
			class Button : public Control {
			public:
				Button(const std::string & name) : Control(name){}
				virtual void press() = 0;
			};
		}
		namespace  ux
		{
			class Button : public Control {
			public:
				Button(const std::string & name) : Control(name) {}
				virtual void press() = 0;
			};
		}
	}
}


#endif // BUTTON_H