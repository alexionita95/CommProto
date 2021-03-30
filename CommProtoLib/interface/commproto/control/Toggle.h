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
				Toggle(const std::string & name) : Control {name}{}

			};
		}
		namespace ux
		{
			class Toggle : public Control {
			public:
				Toggle(const std::string & name) : Control{ name } {}
				virtual void toggle() = 0;

			};
		}
    }
}


#endif // TOGGLE_H