#include <commproto/control/UxControllers.h>


namespace commproto
{
    namespace control
    {
        namespace ux
        {
	        void UxControllers::addController(const std::string& name, const UIControllerHandle& controller)
	        {
		        if (controllers.find(name) != controllers.end())
		        {
			        return;
		        }
		        controllers.emplace(name, controller);
	        }

	        UIControllerHandle UxControllers::getController(const std::string& name)
	        {
		        auto it = controllers.find(name);

		        if (it == controllers.end())
		        {
			        return nullptr;
		        }
		        return it->second;
	        }

	        bool UxControllers::hasUpdate()
	        {
		        bool update = false;
		        for (auto it : controllers)
		        {
			        if (it.second->hasUpdate())
			        {
				        return true;
			        }
		        }
		        return update;
	        }
        }
    }
}