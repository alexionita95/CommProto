#ifndef COMMPROTO_CONTEXT_H
#define COMMPROTO_CONTEXT_H

#include <Common.h>
#include <Socket.h>
#include <Handler.h>
#include <functional>
#include <map>

namespace commproto
{
	namespace data
	{


		class VariableBase;
		using VariableBaseHandle = std::shared_ptr<VariableBase>;
		using VariableCallback = std::function<void(VariableBaseHandle&)>;

		class Context
		{
		public:
			virtual ~Context() = default;

			virtual void subscribe(const uint32_t variableId, VariableCallback & callback) = 0;
			virtual uint32_t registerOutVariable(const VariableBaseHandle & variable) = 0;
			virtual uint32_t registerInVariable(const VariableBaseHandle & variable) = 0;

			virtual void notifyOut(const Message& msg) = 0;
			virtual void notifyIn(const uint32_t variableId) = 0;

			virtual VariableBaseHandle get(uint32_t variableId) = 0;
			virtual uint32_t getMessageType() = 0;
		};

		using ContextHandle = std::shared_ptr<Context>;

		enum class ValueType : uint8_t
		{
			integer32,
			string,
			real32,
		};

		class VariableBase
		{
		public:
			VariableBase(const ValueType type, const ContextHandle &ctx)
				: underlyingType{ type }
				, context{ ctx }
				, index{ 0 }
			{

			}
			void setIndex(const uint32_t index_)
			{
				index = index_;
			}

			ValueType getType() const
			{
				return underlyingType;
			}


			virtual void printValue() = 0;

			virtual Message serialize() = 0;
			virtual ~VariableBase() = default;

		protected:
			ValueType underlyingType;
			ContextHandle context;
			uint32_t index;
		};




		class ContextImpl : public Context
		{
		public:
			ContextImpl(const socketlib::SocketHandle& socket_, const uint32_t messageId_ = 0)
				: socket{ socket_ }
				, messageId{ messageId_ }
			{

			}

			void notifyOut(const Message& msg) override
			{
				socket->sendBytes(msg);
			}
			uint32_t getMessageType() override
			{
				return messageId;
			}
			void subscribe(const uint32_t variableId, VariableCallback & callback) override
			{

				//TODO: Add mechanism for sending the fact that there are new outVariables
				/*
				if (variableId >= outVariables.size())
				{
					return;
				}
				*/
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

			void notifyIn(const uint32_t variableId) override
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

			//your problem is register calls notify which makes some weird loop, you need to make handler -> add var unidirectional
			//another idea would be to separate invariables and outvariables

			uint32_t registerOutVariable(const VariableBaseHandle & variable) override
			{
				uint32_t index = outVariables.size();
				outVariables.emplace_back(variable);


				notifyOut(variable->serialize());

				
				return index;
			}

			uint32_t registerInVariable(const VariableBaseHandle & variable) override
			{
				printf("Registering a variable...");
				uint32_t index = inVariables.size();
				variable->printValue();
				inVariables.push_back(variable);
				inVariables[index]->printValue();
				notifyIn(index);

				return index;
			}

			VariableBaseHandle get(uint32_t variableId) override
			{
				if (variableId >= inVariables.size())
				{
					return VariableBaseHandle();
				}

				return inVariables[variableId];
			}

		private:
			socketlib::SocketHandle socket;
			std::map<uint32_t, std::vector<VariableCallback>> callbacks;
			std::vector<VariableBaseHandle> outVariables;
			std::vector<VariableBaseHandle> inVariables;
			const uint32_t messageId;
		};


	}
}
#endif