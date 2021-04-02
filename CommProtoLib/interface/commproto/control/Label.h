#ifndef LABEL_H
#define LABEL_H
#include <commproto/control/Control.h>

namespace commproto
{
    namespace control
	{
		namespace endpoint {
			class Label : public Control 
			{
			public:
				Label(const std::string & name, const uint32_t id) : Control{name, id}
				{
					
				}
				virtual void setText(const std::string & text) = 0;
			};
		}
		namespace ux
		{
			class Label : public Control
			{
			public:
				Label(const std::string & name, const uint32_t id) : Control{ name, id }
				{

				}
			};
		}
    }
}


#endif // LABEL_H