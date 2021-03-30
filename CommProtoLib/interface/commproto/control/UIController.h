#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include <commproto/control/Control.h>

namespace commproto
{
    namespace control{
		namespace endpoint {
			class UIController : public Control
			{
			public:
				UIController(const std::string & name) : Control{name}{}
				virtual ~UIController() = default;
				virtual void addControl(const std::string &name, const ControlHandle & control) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}
    }
}


#endif // UICONTROLLER_H