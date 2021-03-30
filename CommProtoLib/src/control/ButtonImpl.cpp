#include "ButtonImpl.h"

namespace commproto
{
	namespace control {
		namespace ux {
			std::string ButtonImpl::getUx() const
			{
				return generator->generate(*this);
			}
		}
	}
}