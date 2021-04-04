#ifndef BUTTON_IMPL_H
#define BUTTON_IMPL_H
#include <functional>
#include <commproto/control/Button.h>
#include <commproto/messages/DoublePropertyChain.h>
#include <commproto/messages/SinglePropertyChain.h>
#include <commproto/parser/Handler.h>
#include <commproto/control/UIController.h>

namespace commproto
{
	namespace control {

		namespace endpoint {

			MAKE_DOUBLE_PROP_MESSAGE(ButtonMessage, uint32_t, std::string);

			using ButtonParser = messages::DoublePropertyParser< uint32_t, std::string>;
			using ButtonSerializer = messages::DoublePropertySerializer<uint32_t, std::string>;

			class ButtonHandler : public parser::Handler
			{
			public:
				ButtonHandler(const ux::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
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

		namespace ux {

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;

			MAKE_SINGLE_PROP_MESSAGE(PressButtonMessage, uint32_t);

			using PressButtonParser = messages::SinglePropertyParser<uint32_t>;
			using PressButtonSerializer = messages::SinglePropertySerializer<uint32_t>;

			class PressButtonHandler : public parser::Handler
			{
			public:
				PressButtonHandler(const endpoint::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				endpoint::UIControllerHandle controller;
			};

			class ButtonImpl : public Button {
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

	DEFINE_DATA_TYPE(control::endpoint::ButtonMessage);
	DEFINE_DATA_TYPE(control::ux::PressButtonMessage);

}


#endif // BUTTON_IMPL_H