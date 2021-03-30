#include "UIControllerImpl.h"

namespace commproto
{
	namespace control {
		namespace endpoint {
			void UIControllerImpl::addControl(const std::string& name, const ControlHandle& control)
			{
				if (controls.find(name) != controls.end())
				{
					return;
				}
				controls.emplace(name, control);
			}

			Message UIControllerImpl::serialize() const
			{
				Message result;
				for (auto it = controls.begin(); it != controls.end(); ++it)
				{
					Message controlSerialized = it->second->serialize();
					result.insert(result.end(), controlSerialized.begin(), controlSerialized.end());
				}
				return result;
			}
		}
	}
}