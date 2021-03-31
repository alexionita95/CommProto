#include "UIControllerImpl.h"
#include <sstream>

namespace commproto
{
	namespace control {
		namespace endpoint {
			UIControllerImpl::UIControllerImpl(const std::string& name, const messages::TypeMapperHandle & mapper)
				: UIController{ name }
				, provider{ mapper }
			{
			}

			void UIControllerImpl::addControl(const ControlHandle& control)
			{
				const std::string name = control->getName();
				if (controls.find(name) != controls.end())
				{
					return;
				}
				controls.emplace(name, control);
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

			ControlHandle UIControllerImpl::getControl(const std::string& name)
			{
				auto it = controls.find(name);
				if (it == controls.end())
				{
					return nullptr;
				}
				return  it->second;
			}
		}

		namespace ux
		{

			UIControllerImpl::UIControllerImpl(const std::string& name, const std::string& connectionName_, const messages::TypeMapperHandle & mapper, const sockets::SocketHandle & socket_)
				: UIController{ name }
				, connectionName{ connectionName_ }
				, provider{ mapper }
				, socket{ socket_ }
			{
			}

			void UIControllerImpl::addControl(const ControlHandle& control)
			{
				const std::string name = control->getName();
				if (controls.find(name) != controls.end())
				{
					return;
				}
				controls.emplace(name, control);
			}

			std::string UIControllerImpl::getConnectionName()
			{
				return connectionName;
			}

			std::string UIControllerImpl::getUx() const
			{
				std::stringstream stream;
				for (auto it = controls.begin(); it != controls.end(); ++it)
				{
					stream << it->second->getUx();
				}
				return stream.str();
			}

			void UIControllerImpl::send(Message msg)
			{
				socket->sendBytes(msg);
			}


			UIControllerImpl::~UIControllerImpl()
			{
				controls.clear();
			}

			IdProvider& UIControllerImpl::getIdProvider()
			{
				return provider;
			}
		}
	}
}
