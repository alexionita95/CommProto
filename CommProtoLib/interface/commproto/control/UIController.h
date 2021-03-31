#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include <commproto/control/Control.h>

namespace commproto
{
	namespace control {
		namespace endpoint {

			class IdProvider;

			class UIController : public Control
			{
			public:
				UIController(const std::string & name) : Control{ name } {}
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void addControl(const ControlHandle & control) = 0;
				virtual ControlHandle getControl(const std::string & name) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}

		namespace ux {

			class IdProvider;


			class UIController : public Control
			{
			public:
				UIController(const std::string & name) : Control{ name } {}
				virtual std::string getConnectionName() = 0;
				virtual uint32_t getConnectionId() = 0;
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
				virtual void addControl(const ControlHandle & control) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}
	}
}


#endif // UICONTROLLER_H