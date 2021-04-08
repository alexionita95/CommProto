#ifndef UX_CONTROLLERS_H
#define UX_CONTROLLERS_H
#include <string>
#include <map>
#include <commproto/control/UIController.h>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class UxControllers
			{
			public:
				UxControllers()
					: update(true)
				{
				}

				void addController(const std::string& name, const UIControllerHandle& controller);
				void removeController(const std::string & name);
				UIControllerHandle getController(const std::string& name);
				bool hasUpdate();

			private:
				std::map<std::string, commproto::control::ux::UIControllerHandle> controllers;
				bool update;
			};

			using UxControllersHandle = std::shared_ptr<UxControllers>;
		}
	}

}

#endif //UX_CONTROLLERS_H
