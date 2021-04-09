#ifndef UICONTROLLER_H
#define UICONTROLLER_H
#include <commproto/control/Control.h>
#include <commproto/control/ControlCollection.h>

namespace commproto
{
	namespace control {
		namespace endpoint {

			class IdProvider;

			class UIController : public Control, public ControlCollection
			{
			public:
				UIController(const std::string & name) : Control{ name, 0 } {}
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}

		namespace ux {

			class IdProvider;


			class UIController : public Control, public ControlCollection
			{
			public:
				UIController(const std::string & name) : Control{ name,0 } {}
				virtual std::string getConnectionName() = 0;
				virtual uint32_t getConnectionId() = 0;
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}
	}
}


#endif // UICONTROLLER_H