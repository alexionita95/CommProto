#ifndef UICONTROLLER_IMPL_H
#define UICONTROLLER_IMPL_H
#include <commproto/control/UIController.h>
#include <map>

namespace commproto
{
	namespace control {
		namespace endpoint
		{
			class UIControllerImpl : public UIController
			{
			public:
				UIControllerImpl(const std::string & name) : UIController{ name } {}
				void addControl(const std::string& name, const ControlHandle& control) override;
				Message serialize() const override;
			private:
				std::map<std::string, ControlHandle> controls;
			};
		}
	}
}


#endif // UICONTROLLER_IMPL_H