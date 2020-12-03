#ifndef COMMPROTO_CONTEXT_IMPL_H
#define COMMPROTO_CONTEXT_IMPL_H

#include <commproto/variable/Context.h>
#include <map>

namespace commproto
{
	namespace variable
	{

		class ContextImpl : public Context
		{
		public:
			ContextImpl(const sockets::SocketHandle& socket_, const uint32_t messageId_ = 0, const uint32_t mappingMessageId_ =0);
			void notifyOut(const uint32_t variableId) override;
			uint32_t getVarTypeId() override;
			uint32_t getMappingType() override;
			bool subscribe(const uint32_t variableId, VariableCallback& callback) override;
			bool subscribe(const std::string& name, VariableCallback& callback) override;
			void notifyIn(const uint32_t variableId) override;
			int32_t registerOutVariable(const VariableBaseHandle& variable, const std::string& name = {}) override;
			int32_t registerInVariable(const VariableBaseHandle& variable, const std::string& name = {}) override;
			bool registerMapping(const std::string & name, const uint32_t id) override;
			VariableBaseHandle get(uint32_t variableId) override;
			VariableBaseHandle get(const std::string & name) override;			
			int32_t getVariableId(const std::string & name) override;

		private:

			void internalRegisterOut(const VariableBaseHandle & variable);
			void internalRegisterIn(const VariableBaseHandle & variable);

			void moveCallbacksFromCache(const std::string& name, const uint32_t id);

			sockets::SocketHandle socket;

			std::map<uint32_t, std::vector<VariableCallback>> callbacks;
			std::map<std::string, std::vector<VariableCallback>> nameCallbackCache;

			std::map<std::string, uint32_t> inVariableMapping;
			std::map<std::string, uint32_t> outVariableMapping;

			std::vector<VariableBaseHandle> outVariables;
			std::vector<VariableBaseHandle> inVariables;

			const uint32_t messageId;
			const uint32_t mappingMessageId;
		};
	}
}

#endif //COMMPROTO_CONTEXT_IMPL_H