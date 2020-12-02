#include <commproto/variable/ContextImpl.h>

namespace commproto
{
	namespace variable
	{
		ContextImpl::ContextImpl(const sockets::SocketHandle& socket_, const uint32_t messageId_)
			: socket{socket_}
		    , messageId{messageId_}
		{
		}

		void ContextImpl::notifyOut(const uint32_t variableId)
		{

			if(variableId >= outVariables.size())
			{
				return;
			}
			socket->sendBytes(outVariables[variableId]->serialize());
		}

		uint32_t ContextImpl::getMessageType()
		{
			return messageId;
		}

		void ContextImpl::subscribe(const uint32_t variableId, VariableCallback& callback)
		{
			auto it = callbacks.find(variableId);

			if (it == callbacks.end())
			{
				auto cbs = std::vector<VariableCallback>();
				cbs.emplace_back(callback);
				callbacks.emplace(variableId, cbs);
				return;
			}

			it->second.emplace_back(callback);
		}

		void ContextImpl::notifyIn(const uint32_t variableId)
		{
			auto it = callbacks.find(variableId);

			if (it == callbacks.end())
			{
				return;
			}

			for (auto cb : it->second)
			{
				cb(inVariables[it->first]);
			}
		}

		uint32_t ContextImpl::registerOutVariable(const VariableBaseHandle& variable)
		{
			uint32_t index = outVariables.size();
			outVariables.emplace_back(variable);
			variable->setIndex(index);
			notifyOut(index);
			return index;
		}

		uint32_t ContextImpl::registerInVariable(const VariableBaseHandle& variable)
		{
			uint32_t index = inVariables.size();
			inVariables.push_back(variable);
			variable->setIndex(index);
			notifyIn(index);

			return index;
		}

		VariableBaseHandle ContextImpl::get(uint32_t variableId)
		{
			if (variableId >= inVariables.size())
			{
				return VariableBaseHandle();
			}

			return inVariables[variableId];
		}
	}
}
