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
			ContextImpl(const sockets::SocketHandle& socket_, const uint32_t messageId_ = 0);

			void notifyOut(const uint32_t variableId) override;

			uint32_t getMessageType() override;

			void subscribe(const uint32_t variableId, VariableCallback& callback) override;

			void notifyIn(const uint32_t variableId) override;

			uint32_t registerOutVariable(const VariableBaseHandle& variable) override;

			uint32_t registerInVariable(const VariableBaseHandle& variable) override;

			VariableBaseHandle get(uint32_t variableId) override;

		private:
			sockets::SocketHandle socket;
			std::map<uint32_t, std::vector<VariableCallback>> callbacks;
			std::vector<VariableBaseHandle> outVariables;
			std::vector<VariableBaseHandle> inVariables;
			const uint32_t messageId;
		};
	}
}

#endif //COMMPROTO_CONTEXT_IMPL_H