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
			virtual void subscribe(const uint32_t id, VariableCallback & callback) = 0;

			//subscribe for notifications when an <in>variable changes using the variable's name
			virtual void subscribe(const std::string & name, VariableCallback & callback) = 0;

			//register a local variable this device can modify 
			virtual uint32_t registerOutVariable(const VariableBaseHandle & variable, const std::string & name = {}) = 0;

			//register a variable received from external contexts
			virtual uint32_t registerInVariable(const VariableBaseHandle & variable, const std::string & name = {}) = 0;

			//send the other device an update about a local variable
			virtual void notifyOut(const uint32_t id) = 0;

			//notify the subscribers of an incoming variable change
			virtual void notifyIn(const uint32_t id) = 0;
			
			//get a handle to a <in>variable based on an id
			virtual VariableBaseHandle get(uint32_t id) = 0;

			//get a handle to an <in>variable based on a name
			virtual VariableBaseHandle get(const std::string & name) = 0;

			//get the message type id for variable messages
			virtual uint32_t getVarTypeId() = 0;

			//get the message type id for variable name mapping
			virtual uint32_t getMappingType() = 0;
		};

	}
}
#endif