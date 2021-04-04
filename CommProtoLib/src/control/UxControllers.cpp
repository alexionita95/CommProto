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
				update = true;
		        controllers.emplace(name, controller);
	        }

	        void UxControllers::removeController(const std::string& name)
	        {
				auto it = controllers.find(name);
				if(it == controllers.end())
				{
					return;
				}
				update = true;
				controllers.erase(it);
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
				if(update)
				{
					update = false;
					return true;
				}
		        for (auto it : controllers)
		        {
			        if (it.second->hasUpdate())
			        {
				        return true;
			        }
		        }
		        return false;
	        }
        }
    }
}