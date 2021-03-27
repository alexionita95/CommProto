#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include <commproto/control/Control.h>

namespace commproto
{
    namespace control{
        class UIController
        {
            public:
	        virtual ~UIController() = default;
	        virtual void addControl(const std::string &name, const ControlHandle & control) = 0;
		};
    }
}


#endif // UICONTROLLER_H