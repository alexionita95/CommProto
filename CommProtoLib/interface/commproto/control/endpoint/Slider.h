#ifndef ENDPOINT_SLIDER_H
#define ENDPOINT_SLIDER_H
#include <commproto/control/endpoint/Control.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			class Slider : public Control {
			public:
				Slider(const std::string & name, const uint32_t id) : Control{ name,id } {}
				virtual void setValue(const float) = 0;
			};

			using SliderHandle = std::shared_ptr<Slider>;
		}
	}
}


#endif // SLIDER_H