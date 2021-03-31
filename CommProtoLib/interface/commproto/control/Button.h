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
			using ButtonHandle = std::shared_ptr<Button>;
		}
		namespace  ux
		{
			class Button : public Control {
			public:
				Button(const std::string & name) : Control(name) {}
				virtual void press() = 0;
			};
			using ButtonHandle = std::shared_ptr<Button>;
		}
	}
}


#endif // BUTTON_H