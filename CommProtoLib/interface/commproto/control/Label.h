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
				Label(const std::string & name) : Control{name}
				{
					
				}
				virtual void setText(const std::string & text) = 0;
			};
		}
		namespace ux
		{
			class Label : public Control {
			public:
				Label(const std::string & name) : Control{ name }
				{

				}
			};
		}
    }
}


#endif // LABEL_H