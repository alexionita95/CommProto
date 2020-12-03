#include <commproto/variable/ContextImpl.h>

namespace commproto
{
	namespace variable
	{
		ContextImpl::ContextImpl(const sockets::SocketHandle& socket_, const uint32_t messageId_, const uint32_t mappingMessageId_)
			: socket{ socket_ }
			, messageId{ messageId_ }
			, mappingMessageId{ mappingMessageId_ }
		{
		}

		void ContextImpl::notifyOut(const uint32_t variableId)
		{

			if (variableId >= outVariables.size())
			{
				return;
			}
			socket->sendBytes(outVariables[variableId]->serialize());
		}

		uint32_t ContextImpl::getVarTypeId()
		{
			return messageId;
		}

		uint32_t ContextImpl::getMappingType()
		{
			return mappingMessageId;
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

		void ContextImpl::subscribe(const std::string& name, VariableCallback& callback)
		{
			const auto it = inVariableMapping.find(name);
			if (it == inVariableMapping.end())
			{
				return;
			}

			subscribe(it->second, callback);
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

		void ContextImpl::internalRegisterIn(const VariableBaseHandle& variable)
		{
			uint32_t index = outVariables.size();
			outVariables.emplace_back(variable);
			variable->setIndex(index);
			notifyOut(index);
		}

		void ContextImpl::internalRegisterOut(const VariableBaseHandle& variable)
		{
			uint32_t index = inVariables.size();
			inVariables.push_back(variable);
			variable->setIndex(index);
			notifyIn(index);
		}

		uint32_t ContextImpl::registerOutVariable(const VariableBaseHandle & variable, const std::string & name)
		{
			std::string nameStr(name);

			if (OutVariableMapping.find(nameStr) != OutVariableMapping.end())
			{
				return 0;
			}

			uint32_t id = registerOutVariable(variable);
			OutVariableMapping.emplace(nameStr, id);
			return id;
		}

		uint32_t ContextImpl::registerInVariable(const VariableBaseHandle & variable, const std::string & name)
		{
			std::string nameStr(name);

			if (inVariableMapping.find(nameStr) != inVariableMapping.end())
			{
				return 0;
			}

			uint32_t id = registerInVariable(variable);
			inVariableMapping.emplace(nameStr, id);
			return id;
		}

		VariableBaseHandle ContextImpl::get(uint32_t variableId)
		{
			if (variableId >= inVariables.size())
			{
				return VariableBaseHandle();
			}

			return inVariables[variableId];
		}

		VariableBaseHandle ContextImpl::get(const std::string& name)
		{
			const auto it = inVariableMapping.find(name);
			if (it == inVariableMapping.end())
			{
				return VariableBaseHandle();
			}
			return inVariables[it->second];
		}
	}
}
