#ifndef TOGGLE_H
#define TOGGLE_H
#include <commproto/control/Control.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			class Toggle : public Control {
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name, id } {}

			};
		}
		namespace ux
		{
			class Toggle : public Control {
			public:
				Toggle(const std::string & name, const uint32_t id) : Control{ name , id } {}
				virtual void toggle() = 0;

			};
		}
	}
}


#endif // TOGGLE_H