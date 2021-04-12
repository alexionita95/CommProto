#ifndef UX_CONTROL_H
#define UX_CONTROL_H

#include <commproto/common/Common.h>
#include <memory>
#include <commproto/control/BaseControl.h>

namespace commproto
{
	namespace control {

		namespace ux
		{
			class Control : public BaseControl {
			public:
				Control(const std::string & name, const uint32_t id) : BaseControl(name,id) {}
				virtual ~Control() = default;
				virtual std::string getUx() = 0;
			};
			using ControlHandle = std::shared_ptr<Control>;
		}


	}
}


#endif // UX_CONTROL_H