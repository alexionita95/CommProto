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
				Label(const std::string & name, const uint32_t id) : Control{ name, id }
				{

				}
				virtual void setText(const std::string & text) = 0;
			};

			using LabelHandle = std::shared_ptr<Label>;
		}
		namespace ux
		{
			class Label : public Control
			{
			public:
				Label(const std::string & name, const uint32_t id) : Control{ name, id }
				{

				}
				virtual void setText(const std::string & text) = 0;
				virtual std::string getText() const = 0;
			};
			using LabelHandle = std::shared_ptr<Label>;
		}
	}
}


#endif // LABEL_H