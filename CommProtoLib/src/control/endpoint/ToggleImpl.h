#ifndef ENDPOINT_TOGGLE_IMPL_H
#define ENDPOINT_TOGGLE_IMPL_H

#include <commproto/control/endpoint/Toggle.h>
#include <commproto/control/endpoint/UIController.h>
#include <commproto/control/ToggleChains.h>

namespace commproto
{
    namespace control
	{
		namespace endpoint
		{
			class ToggleHandler : public parser::Handler
			{
			public:
				ToggleHandler(const UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				UIControllerHandle controller;
			};


			class ToggleImpl : public Toggle {
			public:
				ToggleImpl(const std::string& name, uint32_t id,  const uint32_t messageId, const ToggleAction& action_, const bool defaultState = false);

				Message serialize() const override;
				void toggle() override;
			private:
				uint32_t messageId;
				ToggleAction action;
				bool state;
			};
		}
    }

}


#endif // ENDPOINT_TOGGLE_IMPL_H