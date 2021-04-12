#ifndef UX_TOGGLE_IMPL_H
#define UX_TOGGLE_IMPL_H

#include <commproto/control/ux/Toggle.h>
#include <commproto/control/ux/UIController.h>
#include <commproto/control/ToggleChains.h>

namespace commproto
{
    namespace control
	{

		namespace ux
		{

			class ToggleHandler : public parser::Handler
			{
			public:
				ToggleHandler(const ux::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
			};

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;

			class ToggleImpl : public Toggle {
			public:
				ToggleImpl(const std::string& name, uint32_t id, const uint32_t messageId, const GeneratorHandle& generator_, const bool defaultState = false);
				std::string getUx() override;
				void toggle() override;
				bool getState() const override;
			private:
				GeneratorHandle generator;
				const uint32_t messageId;
				bool state;
			};
		}
    }
}


#endif // UX_TOGGLE_IMPL_H