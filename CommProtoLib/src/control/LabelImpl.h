#ifndef LABEL_IMPL_H
#define LABEL_IMPL_H
#include <commproto/control/Label.h>
#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/messages/DoublePropertyChain.h>
#include <commproto/control/UIController.h>

namespace commproto
{
	namespace control
	{
		namespace endpoint
		{

			MAKE_TRIPLE_PROP_MESSAGE(LabelMessage, uint32_t, std::string, std::string);

			using LabelParser = messages::TriplePropertyParser<uint32_t, std::string, std::string>;
			using LabelSerializer = messages::TriplePropertySerializer<uint32_t, std::string, std::string>;

			class LabelHandler : public parser::Handler
			{
			public:
				LabelHandler(const ux::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
			};

			MAKE_DOUBLE_PROP_MESSAGE(LabelUpdateMessage, uint32_t, std::string);

			using LabelUpdateParser = messages::DoublePropertyParser<uint32_t, std::string>;
			using LabelUpdateSerializer = messages::DoublePropertySerializer<uint32_t, std::string>;

			class LabelUpdateHandler : public parser::Handler
			{
			public:
				LabelUpdateHandler(const ux::UIControllerHandle& controller_);
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
			};


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
		namespace ux
		{

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;

			class LabelImpl : public Label {
			public:
				LabelImpl(const std::string& name, uint32_t id, const std::string& text_, const GeneratorHandle & generator);

				std::string getUx() override;
				void setText(const std::string& text) override;
				std::string getText() const override;
			private:
				std::string text;
				GeneratorHandle generator;
			};
		}
	}
	DEFINE_DATA_TYPE(control::endpoint::LabelMessage);
	DEFINE_DATA_TYPE(control::endpoint::LabelUpdateMessage);
}


#endif // LABEL_IMPL_H