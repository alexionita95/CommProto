#ifndef UX_SLIDER_H
#define UX_SLIDER_H
#include <commproto/control/ux/Control.h>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class Slider : public Control {
			public:
				Slider(const std::string & name, const uint32_t id) : Control{ name ,id } {}
				virtual void setValue(const float) = 0;
			};
			using SliderHandle = std::shared_ptr<Slider>;
		}
	}
}


#endif // UX_SLIDER_H