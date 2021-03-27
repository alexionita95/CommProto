#include "ControlBase.h"

namespace commproto
{
	namespace control {
		ControlBase::ControlBase(const std::string& name_)
			:name{name_}
		{
		}

		std::string ControlBase::getName() const
		{
			return name;
		}
	}
}