#include "UIControllerImpl.h"
#include <sstream>
#include <commproto/service/ServiceChains.h>
#include <commproto/logger/Logging.h>
#include <algorithm>

namespace commproto
{
	namespace control {
		namespace endpoint {
			UIControllerImpl::UIControllerImpl(const std::string& name, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_)
				: UIController{ name }
				, provider(mapper)
				, idCounter{ 1 } // id 0 is reserved for the ui controller itself
				, socket{ socket_ }
			{
			}

			void UIControllerImpl::addControl(const ControlHandle& control)
			{
				LOG_INFO("Adding a control named \"%s\"", control->getName().c_str());
				const uint32_t id = control->getId();
				if (controls.find(id) != controls.end())
				{
					return;
				}
				controls.emplace(id, control);
			}

			Message UIControllerImpl::serialize() const
			{
				Message result;
				for (auto it = controls.begin(); it != controls.end(); ++it)
				{
					Message controlSerialized = it->second->serialize();
					result.insert(result.end(), controlSerialized.begin(), controlSerialized.end());
				}
				for (auto it = notifications.begin(); it != notifications.end(); ++it)
				{
					Message controlSerialized = it->second->serialize();
					result.insert(result.end(), controlSerialized.begin(), controlSerialized.end());
				}
				return result;
			}

			UIControllerImpl::~UIControllerImpl()
			{
				controls.clear();
			}

			IdProvider& UIControllerImpl::getIdProvider()
			{
				return provider;
			}

			ControlHandle UIControllerImpl::getControl(const uint32_t id)
			{
				auto it = controls.find(id);
				if (it == controls.end())
				{
					return nullptr;
				}
				return  it->second;
			}

			uint32_t UIControllerImpl::reserveId()
			{
				return idCounter++;
			}

			void UIControllerImpl::clear()
			{
				controls.clear();
			}

			void UIControllerImpl::send(Message msg)
			{
				if(!socket)
				{
					return;
				}
				socket->sendBytes(msg);
			}

			void UIControllerImpl::addNotification(const NotificationHandle& notification)
			{
				const uint32_t id = notification->getId();
				if(notifications.find(id) != notifications.end())
				{
					return;
				}

				notifications.emplace(id, notification);
			}

			NotificationHandle UIControllerImpl::getNotification(const uint32_t id) const
			{
				auto it = notifications.find(id);
				if(it == notifications.end())
				{
					return nullptr;
				}
				return it->second;
			}

			void UIControllerImpl::displayNotification(const uint32_t id) const
			{
				auto it = notifications.find(id);
				if (it == notifications.end())
				{
					return;
				}
				socket->sendBytes(it->second->serializeDisplay());
			}
		}

		namespace ux
		{

			UIControllerImpl::UIControllerImpl(const std::string& name, const std::string& connectionName_, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_, const uint32_t id)
				: UIController{ name }
				, connectionName{ connectionName_ }
				, provider{ mapper }
				, socket{ socket_ }
				, connectionId{ id }
				, update{ true }
				, hasNotif{false}
			{
			}

			void UIControllerImpl::addControl(const ControlHandle& control)
			{
				LOG_INFO("Adding a control named \"%s\"", control->getName().c_str());
				std::lock_guard<std::mutex> lock(controlMutex);
				update = true;
				const uint32_t id = control->getId();
				if (controls.find(id) != controls.end())
				{
					return;
				}
				controls.emplace(id, control);
			}

			std::string UIControllerImpl::getConnectionName()
			{
				return connectionName;
			}

			std::string UIControllerImpl::getUx()
			{
				std::stringstream stream;
				std::lock_guard<std::mutex> lock(controlMutex);
				for (auto it = controls.begin(); it != controls.end(); ++it)
				{
					stream << it->second->getUx();
				}
				update = false;
				return stream.str();
			}

			void UIControllerImpl::send(Message msg)
			{
				service::SendToMessage sendto(provider.sendToId, connectionId, msg);
				socket->sendBytes(service::SendtoSerializer::serialize(std::move(sendto)));
			}


			UIControllerImpl::~UIControllerImpl()
			{
				controls.clear();
			}

			IdProvider& UIControllerImpl::getIdProvider()
			{
				return provider;
			}

			uint32_t UIControllerImpl::getConnectionId()
			{
				return connectionId;
			}

			ControlHandle UIControllerImpl::getControl(const uint32_t id)
			{
				std::lock_guard<std::mutex> lock(controlMutex);
				auto it = controls.find(id);
				if (it == controls.end())
				{
					return nullptr;
				}
				return  it->second;
			}

			bool UIControllerImpl::hasUpdate()
			{
				return update;
			}

			void UIControllerImpl::clear()
			{
				std::lock_guard<std::mutex> lock(controlMutex);
				controls.clear();
			}

			void UIControllerImpl::notifyUpdate()
			{
				update = true;
			}

			void UIControllerImpl::addNotification(const NotificationHandle& notification)
			{
				const uint32_t id = notification->getId();		
				if (notifications.find(id) != notifications.end())
				{
					return;
				}
				LOG_INFO("Adding notification with id %d", id);
				notifications.emplace(id, notification);
			}

			NotificationHandle UIControllerImpl::getNotification(const uint32_t id) const
			{
				auto it = notifications.find(id);
				if (it == notifications.end())
				{
					return nullptr;
				}
				return it->second;
			}

			void UIControllerImpl::displayNotification(const uint32_t id)
			{
				auto it = notifications.find(id);
				if (it == notifications.end())
				{
					return;
				}
				std::lock_guard<std::mutex> lock(notificationMutex);
                pendingNotifications.push_back(id);
				hasNotif = true;
								
			}

			bool UIControllerImpl::hasNotifications()
			{
				return hasNotif;
			}

			std::string UIControllerImpl::getNotifications()
			{
				std::stringstream stream;
				std::lock_guard<std::mutex> lock(notificationMutex);
				for(auto id : pendingNotifications)
				{
					auto it = notifications.find(id);
					if(it == notifications.end())
					{
						continue;
					}
					stream << it->second->getUx();
				}
				hasNotif = false;
				return stream.str();
			}

			void UIControllerImpl::dismissNotification(const uint32_t id)
			{
				std::lock_guard<std::mutex> lock(notificationMutex);
				auto it = std::find(pendingNotifications.begin(), pendingNotifications.end(),id);
				if(it == pendingNotifications.end())
				{
					return;
				}

				pendingNotifications.erase(it);
			}
		}
	}
}
