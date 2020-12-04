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
			/**
			 * \brief Register a callback when the in variable with the provided id changes. Will still register the callback if the variable is not currently registered in this context.
			 * \return Returns true if the variable is registed and false otherwise.
			 */
			virtual bool subscribe(const uint32_t id, VariableCallback& callback) = 0;
			/**
			* \brief Register a callback when the in variable with the provided name changes. Will still register the callback if the variable is not currently registered in this context.
			 * \return Returns true if the variable is registed and false otherwise.
			*/
			virtual bool subscribe(const std::string& name, VariableCallback& callback) = 0;
			/**
			 * \brief Register a local variable to be sent to the external context.
			 * \param name[optional] An optional name for this variable.
			 * \return Variable's assigned id. Note: If a name is provided and a local variable is also registered with this name, the method will return -1 and this variable will not be registered.
			 */
			virtual int32_t registerOutVariable(const VariableBaseHandle& variable, const std::string& name = {}) = 0;
			/**
			* \brief Register an incoming variable received from external contexts.
			* \param name[optional] An optional name for this variable.
			* \return Variable's assigned id. Note: If a name is provided and a local variable is also registered with this name, the method will return -1 and this variable will not be registered.
			*/
			virtual int32_t registerInVariable(const VariableBaseHandle& variable, const std::string& name = {}) = 0;


			/**
			 * \brief Register a mapping for an incoming variable
			 * \return True if the variable could be registered, false if a variable with that name already exists.
			 */
			virtual bool registerMapping(const std::string & name, const uint32_t id) = 0;
			/**
			 * \brief Notify the external context that a local variable has changed
			 */
			virtual void notifyOut(const uint32_t id) = 0;
			/**
			* \brief Notify the local subscribers that an external variable has changed.
			*/
			virtual void notifyIn(const uint32_t id) = 0;
			/**
			 * \brief Get an internal variable by id.
			 * \return Returns an empty handle if an internal variable with that id does not exist.
			 */
			virtual VariableBaseHandle get(uint32_t id) = 0;
			/**
			* \brief Get an internal variable by name.
			* \return Returns an empty handle if an internal variable with that name does not exist.
			*/
			virtual VariableBaseHandle get(const std::string & name) = 0;
			/**
			 * \brief Get an internal variable's id based on its name
			 * \return The variable's id or -1 if a variable with that name does not exist.
			 */
			virtual int32_t getVariableId(const std::string & name) = 0;
			/**
			 * \brief Get the message type id for variable messages.
			 */
			virtual uint32_t getVarTypeId() = 0;
			/**
			 * \brief Get the message type id for variable name mapping messages.
			 */
			virtual uint32_t getMappingType() = 0;
		};

	}
}
#endif