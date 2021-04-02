#ifndef SLIDER_H
#define SLIDER_H
#include <commproto/control/Control.h>

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
		}
		namespace ux
		{
			class Slider : public Control {
			public:
				Slider(const std::string & name, const uint32_t id) : Control{ name ,id } {}
				virtual void setValue(const float) = 0;
			};
		}
	}
}


#endif // SLIDER_H