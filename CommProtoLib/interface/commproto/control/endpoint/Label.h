#ifndef ENDPOINT_LABEL_H
#define ENDPOINT_LABEL_H
#include <commproto/control/endpoint/Control.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint {
			class Label : public Control
			{
			public:
				Label(const std::string & name, const uint32_t id) : Control{ name, id }
				{

				}
				virtual void setText(const std::string & text) = 0;
			};

			using LabelHandle = std::shared_ptr<Label>;
		}
	}
}


#endif // ENDPOINT_LABEL_H