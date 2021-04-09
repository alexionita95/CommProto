#ifndef NOTIFICATION_IMPL_H
#define NOTIFICATION_IMPL_H
#include <commproto/control/Notification.h>
#include <commproto/messages/TriplePropertyChain.h>
#include <commproto/control/UIController.h>
#include <commproto/messages/SinglePropertyChain.h>
#include <commproto/messages/DoublePropertyChain.h>


namespace commproto
{
	namespace control
	{
		namespace endpoint
		{


			MAKE_TRIPLE_PROP_MESSAGE(NotificationMessage, uint32_t, std::string, std::vector<std::string>);

			using NotificationSerializer = messages::TriplePropertySerializer<uint32_t, std::string, std::vector<std::string>>;
			using NotificationParser = messages::TriplePropertyParser<uint32_t, std::string, std::vector<std::string>>;

			class NotificationHandler : public parser::Handler
			{
			public:
				NotificationHandler(const ux::UIControllerHandle & controller_) : controller{ controller_ } {}
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
			};


			MAKE_SINGLE_PROP_MESSAGE(DisplayNotificationMessage, uint32_t);
			using DisplayNotificationSerializer = messages::SinglePropertySerializer<uint32_t>;
			using DisplayNotificationParser = messages::SinglePropertyParser<uint32_t>;


			class DisplayNotificationHandler : public  parser::Handler
			{
			public:
				DisplayNotificationHandler(const ux::UIControllerHandle & controller_) : controller{ controller_ } {}
				void handle(messages::MessageBase&& data) override;
			private:
				ux::UIControllerHandle controller;
			};

			class NotificationImpl : public Notification
			{
			public:
				NotificationImpl(const std::string& name, uint32_t id, const uint32_t notifId_, const uint32_t displayId_)
					: Notification(name, id)
					, notifId{ notifId_ }
					, displayId{ displayId_ }
				{
				}

				Message serialize() const override;
				void addOption(const std::string& name) override;
				void setAction(const NotificationAction& action) override;
				void execute(const std::string& option) override;
				Message serializeDisplay() override;
			private:
				const uint32_t notifId;
				const uint32_t displayId;
				std::vector<std::string> options;
				NotificationAction action;
			};
		}

		namespace ux
		{

			MAKE_DOUBLE_PROP_MESSAGE(NotificationResponseMessage, uint32_t, std::string);
			using NotificationResponseSerializer = messages::DoublePropertySerializer<uint32_t, std::string>;
			using NotificationResponseParser = messages::DoublePropertyParser<uint32_t, std::string>;


			class NotificationResponseHandler : public parser::Handler
			{
			public:
				NotificationResponseHandler(const endpoint::UIControllerHandle & controller_) : controller{ controller_ } {}
				void handle(messages::MessageBase&& data) override;
			private:
				endpoint::UIControllerHandle controller;
			};

			class Generator;
			using GeneratorHandle = std::shared_ptr<Generator>;

			class NotificationImpl : public Notification
			{
			public:
				NotificationImpl(const std::string& name, uint32_t id, const uint32_t executeId, const std::vector<std::string> & options, const GeneratorHandle& generator_)
					: Notification(name, id)
					, executeId{ executeId }
					, options{ options }
					, generator{ generator_ }
				{
				}

				std::string getUx() override;
				void execute(const std::string& option) override;
				std::vector<std::string> getOptions() const override;
			private:
				const uint32_t executeId;
				const std::vector<std::string> options;
				GeneratorHandle generator;
			};
		}
	}

	DEFINE_DATA_TYPE(control::endpoint::NotificationMessage);
	DEFINE_DATA_TYPE(control::endpoint::DisplayNotificationMessage);
	DEFINE_DATA_TYPE(control::ux::NotificationResponseMessage);
}

#endif //NOTIFICATION_IMPL_H