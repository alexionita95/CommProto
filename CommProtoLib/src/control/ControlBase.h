#ifndef CONTROL_BASE_H
#define CONTROL_BASE_H

#include <commproto/control/Control.h>

namespace commproto
{
	namespace control
	{
		class ControlBase : public Control {
		public:
			ControlBase(const std::string & name);
			std::string getName() const override;
		protected:
			std::string name;
		};

	}
}


#endif // CONTROL_H