#include "ButtonImpl.h"

namespace commproto
{
	namespace control {
		std::string UxButton::getUX() const
		{
			return generator->generate(*this);
		}
	}
}