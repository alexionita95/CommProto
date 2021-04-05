#ifndef UICONTROLLER_IMPL_H
#define UICONTROLLER_IMPL_H

#include <commproto/control/UIController.h>
#include <commproto/sockets/Socket.h>
#include <map>
#include "IdProvider.h"


namespace commproto
{
	namespace control {
		namespace endpoint
		{
			class UIControllerImpl : public UIController
			{
			public:
				UIControllerImpl(const std::string& name, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_);
				void addControl(const ControlHandle& control) override;
				Message serialize() const override;
				~UIControllerImpl();
				IdProvider& getIdProvider() override;
				ControlHandle getControl(const uint32_t id) override;
				uint32_t reserveId() override;
				void clear() override;
				void send(Message msg) override;
			private:
				std::map<uint32_t, ControlHandle> controls;
				IdProvider provider;
				uint32_t idCounter;
				sockets::SocketHandle socket;
			};
		}

		namespace ux
		{
			class UIControllerImpl : public UIController
			{
			public:
				UIControllerImpl(const std::string& name, const std::string& connectionName_, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket, const uint32_t id);
				void addControl(const ControlHandle& control) override;
				std::string getConnectionName() override;
				std::string getUx() override;
				void send(Message msg) override;
				~UIControllerImpl();
				IdProvider& getIdProvider() override;
				uint32_t getConnectionId() override;
				ControlHandle getControl(const uint32_t id) override;
				bool hasUpdate() override;
				void clear() override;
				void notifyUpdate() override;
			private:
				std::map<uint32_t, ControlHandle> controls;
				const std::string connectionName; 
				IdProvider provider;
				sockets::SocketHandle socket;
				uint32_t connectionId;
				std::mutex controlMutex;
				std::atomic_bool update;
			};
		}
	}
}


#endif // UICONTROLLER_IMPL_H