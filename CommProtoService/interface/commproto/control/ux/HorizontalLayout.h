#ifndef UX_HORIZONTAL_LAYOUT_H
#define UX_HORIZONTAL_LAYOUT_H
#include <commproto/control/ux/Control.h>
#include  <commproto/control/ux/ControlCollection.h>

namespace commproto
{
    namespace control
    {
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

#endif //UX_HORIZONTAL_LAYOUT_H