#ifndef HORIZONTAL_LAYOUT_H
#define HORIZONTAL_LAYOUT_H
#include <commproto/control/Control.h>
#include  <commproto/control/ControlCollection.h>

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
        namespace ux
        {
			class VerticalLayout : public Control, public ControlCollection
			{
			public:
				VerticalLayout(const std::string& name, uint32_t id)
					: Control(name, id)
				{
				}
			};
        }
    }
}

#endif //HORIZONTAL_LAYOUT_H