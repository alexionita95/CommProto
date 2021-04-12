#ifndef ENDPOINT_HORIZONTAL_LAYOUT_H
#define ENDPOINT_HORIZONTAL_LAYOUT_H
#include <commproto/control/endpoint/Control.h>
#include  <commproto/control/endpoint/ControlCollection.h>

namespace commproto
{
    namespace control
    {
        namespace endpoint
        {
            class HorizontalLayout : public Control, public ControlCollection
            {
            public:
	            HorizontalLayout(const std::string& name, uint32_t id)
		            : Control(name, id)
	            {
	            }
            };
        }
    }
}

#endif //ENDPOINT_HORIZONTAL_LAYOUT_H