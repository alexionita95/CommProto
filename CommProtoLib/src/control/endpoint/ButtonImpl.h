#ifndef ENDPOINT_BUTTON_IMPL_H
#define ENDPOINT_BUTTON_IMPL_H

#include <functional>
#include <commproto/control/endpoint/Button.h>
#include <commproto/control/endpoint/UIController.h>
#include <commproto/control/ButtonChains.h>

namespace commproto
{
	namespace control {

		namespace endpoint {

			class PressButtonHandler : public parser::Handler
			{
			public:
				PressButtonHandler(const UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				UIControllerHandle controller;
			};

			class ButtonImpl : public Button {
			public:
				ButtonImpl(const std::string& name, const uint32_t id, const uint32_t buttonId, const ButtonAction& action_);

				void press() override;

				Message serialize() const override;
			private:
				ButtonAction action;
				const uint32_t buttonId;
			};
		}
	}
}


#endif // ENDPOINT_BUTTON_IMPL_H