#include <commproto/variable/ContextImpl.h>
#include <commproto/variable/VariableMappingMessage.h>
#include "Logging.h"

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
			LOG_DEBUG("notifying variable %d", variableId);
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

		bool ContextImpl::subscribe(const uint32_t variableId, VariableCallback& callback)
		{
			auto it = callbacks.find(variableId);

			if (it == callbacks.end())
			{
				auto cbs = std::vector<VariableCallback>();
				cbs.emplace_back(callback);
				callbacks.emplace(variableId, cbs);
				return false;
			}

			it->second.emplace_back(callback);
			return true;
		}

		bool ContextImpl::subscribe(const std::string& name, VariableCallback& callback)
		{
			const auto it = inVariableMapping.find(name);
			if (it == inVariableMapping.end())
			{
				auto it2 = nameCallbackCache.find(name);

				if (it2 == nameCallbackCache.end())
				{
					auto cbs = std::vector<VariableCallback>();
					cbs.emplace_back(callback);
					nameCallbackCache.emplace(name, cbs);
                    return false;
				}
				it2->second.emplace_back(callback);
				return false;
			}
			subscribe(it->second, callback);
			return true;
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

		void ContextImpl::moveCallbacksFromCache(const std::string& name, const uint32_t id)
		{
            LOG_DEBUG("Tryign to get cached callbacks for %s -> %d",name.c_str(),id);
			auto it = nameCallbackCache.find(name);

			if (it != nameCallbackCache.end())
			{
				auto it2 = callbacks.find(id);
				if (it2 != callbacks.end())
				{
                    LOG_DEBUG("All good for mapping %s -> %d",name.c_str(),id);
					it2->second.insert(it2->second.end(), it->second.begin(), it->second.end());
				}
				else
				{
					callbacks.emplace(id, it->second);
				}
				nameCallbackCache.erase(it);
			}
		}

		void ContextImpl::internalRegisterOut(const VariableBaseHandle& variable)
		{
			uint32_t index = outVariables.size();
			outVariables.emplace_back(variable);
			variable->setIndex(index);
			notifyOut(index);
		}

		void ContextImpl::internalRegisterIn(const VariableBaseHandle& variable)
		{
			uint32_t index = inVariables.size();
			inVariables.push_back(variable);
			variable->setIndex(index);
			notifyIn(index);
		}

		int32_t ContextImpl::registerOutVariable(const VariableBaseHandle& variable, const std::string& name)
		{

			uint32_t id = outVariables.size();
			LOG_DEBUG("registering out variable...");
			if (!name.empty())
			{
				
				if (outVariableMapping.find(name) != outVariableMapping.end()) {
					LOG_WARNING("already found a mapping to the name %s",name.c_str());
					return -1;
				}
				LOG_DEBUG(	"registered a mapping %s -> %d", name.c_str(),id);
				outVariableMapping.emplace(name, id);
				socket->sendBytes(VariableMappingSerializer::serialize(std::move(VariableMappingMessage(mappingMessageId,name,id))));

			}
			internalRegisterOut(variable);
			return id;
		}

		int32_t ContextImpl::registerInVariable(const VariableBaseHandle& variable, const std::string& name)
		{
			uint32_t id = inVariables.size();
			if (!name.empty())
			{
				if (inVariableMapping.find(name) != inVariableMapping.end()) {
					return -1;
				}
				inVariableMapping.emplace(name, id);

			}
			internalRegisterIn(variable);
			moveCallbacksFromCache(name, id);

			return id;
		}

		bool ContextImpl::registerMapping(const std::string& name, const uint32_t id)
		{
            LOG_DEBUG("Trying to register mapping for  %s -> %d",name.c_str(),id);
            if (inVariableMapping.find(name) != inVariableMapping.end()) {
				return false;
			}
			inVariableMapping.emplace(name, id);
            moveCallbacksFromCache(name,id);
			return true;
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

		int32_t ContextImpl::getVariableId(const std::string& name)
		{
			auto it = inVariableMapping.find(name);
			if (it == inVariableMapping.end()) {
				return -1;
			}
			return it->second;
		}
	}
}
