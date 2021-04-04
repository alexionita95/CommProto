#ifndef TOGGLE_IMPL_H
#define TOGGLE_IMPL_H

#include <commproto/control/Toggle.h>
#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/control/UIController.h>
#include <commproto/messages/SinglePropertyChain.h>

namespace commproto
{
    namespace control
	{
		namespace endpoint
		{

			MAKE_TRIPLE_PROP_MESSAGE(ToggleMessage, uint32_t, std::string, bool);

			using ToggleParser = messages::TriplePropertyParser<uint32_t, std::string,bool>;
			using ToggleSerializer = messages::TriplePropertySerializer<uint32_t, std::string,bool>;

			class ToggleHandler : public parser::Handler
			{
			public:
				ToggleHandler(const ux::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
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


		namespace ux
		{

			MAKE_SINGLE_PROP_MESSAGE(ToggleMessage, uint32_t);
			using ToggleParser = messages::SinglePropertyParser<uint32_t>;
			using ToggleSerializer = messages::SinglePropertySerializer<uint32_t>;

			class ToggleHandler : public parser::Handler
			{
			public:
				ToggleHandler(const endpoint::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				endpoint::UIControllerHandle controller;
			};

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;

			class ToggleImpl : public Toggle {
			public:
				ToggleImpl(const std::string& name, uint32_t id, const uint32_t messageId, const GeneratorHandle& generator_, const bool defaultState = false);
				std::string getUx() override;
				void toggle() override;
			private:
				GeneratorHandle generator;
				const uint32_t messageId;
				bool state;
			};
		}
    }
	DEFINE_DATA_TYPE(control::endpoint::ToggleMessage);
	DEFINE_DATA_TYPE(control::ux::ToggleMessage);
}


#endif // TOGGLE_IMPL_H