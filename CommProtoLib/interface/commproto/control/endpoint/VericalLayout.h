#ifndef ENDPOINT_VERTICAL_LAYOUT_H
#define ENDPOINT_VERTICAL_LAYOUT_H
#include <commproto/control/endpoint/Control.h>
#include  <commproto/control/endpoint/ControlCollection.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
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

#endif //ENDPOINT_VERTICAL_LAYOUT_H