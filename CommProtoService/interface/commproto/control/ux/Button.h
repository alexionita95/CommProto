#ifndef UX_BUTTON_H
#define UX_BUTTON_H
#include <commproto/control/ux/Control.h>

namespace commproto
{
	namespace control
	{
		namespace  ux
		{
			class Button : public Control 
			{
			public:
				Button(const std::string & name, const uint32_t id) : Control(name,id) {}
				virtual void press() = 0;
			};
			using ButtonHandle = std::shared_ptr<Button>;
		}
	}
}


#endif // UX_BUTTON_H