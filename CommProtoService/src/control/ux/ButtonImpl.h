#ifndef UX_BUTTON_IMPL_H
#define Ux_BUTTON_IMPL_H

#include <commproto/control/ux/Button.h>
#include <commproto/control/ux/UIController.h>
#include <commproto/control/ButtonChains.h>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class ButtonHandler : public parser::Handler
			{
			public:
				ButtonHandler(const UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				UIControllerHandle controller;

			};

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;


			class ButtonImpl : public Button
			{
			public:
				ButtonImpl(const std::string& name_, const uint32_t id, const uint32_t pressId, const GeneratorHandle& generator_);

				void press() override;

				std::string getUx() override;
			private:
				GeneratorHandle generator;
				const uint32_t pressId;
			};
		}

	}
}

#endif // UX_BUTTON_IMPL_H