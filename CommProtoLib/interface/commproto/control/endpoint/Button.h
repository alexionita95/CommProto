#ifndef ENDPOINT_BUTTON_H
#define ENDPOINT_BUTTON_H
#include <commproto/control/endpoint/Control.h>
#include <functional>

namespace commproto
{
	namespace control 
	{
		namespace  endpoint 
		{
			using ButtonAction = std::function<void()>;
			class Button : public Control {
			public:
				Button(const std::string & name, const uint32_t id) : Control(name, id) {}
				virtual void press() = 0;
			};
			using ButtonHandle = std::shared_ptr<Button>;
		}
	}
}


#endif // ENDPOINT_BUTTON_H