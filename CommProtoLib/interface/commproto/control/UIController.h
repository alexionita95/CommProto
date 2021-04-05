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
				UIController(const std::string & name) : Control{ name, 0 } {}
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void addControl(const ControlHandle & control) = 0;
				virtual uint32_t reserveId() = 0;
				virtual ControlHandle getControl(const uint32_t id) = 0;
				virtual void clear() = 0;
				virtual void send(Message msg) = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}

		namespace ux {

			class IdProvider;


			class UIController : public Control
			{
			public:
				UIController(const std::string & name) : Control{ name,0 } {}
				virtual std::string getConnectionName() = 0;
				virtual uint32_t getConnectionId() = 0;
				virtual ~UIController() = default;
				virtual IdProvider& getIdProvider() = 0;
				virtual void send(Message msg) = 0;
				virtual void addControl(const ControlHandle & control) = 0;
				virtual bool hasUpdate() = 0;
				virtual ControlHandle getControl(const uint32_t id) = 0;
				virtual void clear() = 0;
				virtual void notifyUpdate() = 0;
			};

			using UIControllerHandle = std::shared_ptr<UIController>;
		}
	}
}


#endif // UICONTROLLER_H