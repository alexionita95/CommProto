#include "UIControllerImpl.h"
#include <sstream>
#include <commproto/service/ServiceChains.h>

namespace commproto
{
	namespace control {
		namespace endpoint {
			UIControllerImpl::UIControllerImpl(const std::string& name, const messages::TypeMapperHandle & mapper)
				: UIController{ name }
				, provider(mapper)
				, idCounter{1} // id 0 is reserved for the ui controller itself
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
		}

		namespace ux
		{

			UIControllerImpl::UIControllerImpl(const std::string& name, const std::string& connectionName_, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_, const uint32_t id)
				: UIController{ name }
				, connectionName{ connectionName_ }
				, provider{ mapper }
				, socket{ socket_ }
				, connectionId{ id }
			{
			}

			void UIControllerImpl::addControl(const ControlHandle& control)
			{
				LOG_INFO("Adding a control named \"%s\"", control->getName().c_str());
				std::lock_guard<std::mutex> lock(controlMutex);
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
		}
	}
}
