#ifndef COMMPROTO_CONTEXT_H
#define COMMPROTO_CONTEXT_H

#include <commproto/sockets/Socket.h>
#include <commproto/variable/VariableBase.h>
#include <functional>

namespace commproto
{
	namespace variable
	{

		using VariableCallback = std::function<void(VariableBaseHandle&)>;

		//used to synchronize variables between two communicating devices
		class Context
		{
		public:
			virtual ~Context() = default;

			//subscribe for notifications when an <in>variable changes
			virtual void subscribe(const uint32_t variableId, VariableCallback & callback) = 0;
			
			//register a variable intended for being sent to the other device
			virtual uint32_t registerOutVariable(const VariableBaseHandle & variable) = 0;

			//register a variable received from the other device
			virtual uint32_t registerInVariable(const VariableBaseHandle & variable) = 0;

			//send the other device an update about a local variable
			virtual void notifyOut(const uint32_t variableId) = 0;
			
			//notify the subscribers of an incoming variable change
			virtual void notifyIn(const uint32_t variableId) = 0;

			virtual VariableBaseHandle get(uint32_t variableId) = 0;
			virtual uint32_t getMessageType() = 0;
		};

	}
}
#endif