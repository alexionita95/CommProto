#ifndef ENDPOINT_CONTROL_H
#define ENDPOINT_CONTROL_H

#include <commproto/common/Common.h>
#include <memory>
#include <commproto/control/BaseControl.h>

namespace commproto
{
	namespace control {

		namespace endpoint {
			class Control : public BaseControl {
			public:
				Control(const std::string & name, const uint32_t id) : BaseControl(name, id){}
				virtual ~Control() = default;
				virtual Message serialize() const = 0;
			};
			using ControlHandle = std::shared_ptr<Control>;
		}

	}
}


#endif // ENDPOINT_CONTROL_H