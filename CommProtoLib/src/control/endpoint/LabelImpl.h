#ifndef ENDPOINT_LABEL_IMPL_H
#define ENDPOINT_LABEL_IMPL_H
#include <commproto/control/endpoint/Label.h>
#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/messages/DoublePropertyChain.h>
#include <commproto/control/endpoint/UIController.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{
			class LabelImpl : public Label {
			public:
				LabelImpl(const std::string& name, uint32_t id, const uint32_t messageId_, const uint32_t updateMsgId_, const std::string& defaultText, const UIControllerHandle& controller_);

				Message serialize() const override;
				void setText(const std::string& text) override;
			private:
				const uint32_t messageId;
				const uint32_t updateId;
				std::string text;
				UIControllerHandle controller;
			};


		}
	}
}


#endif // LABEL_IMPL_H