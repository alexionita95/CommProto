#include <commproto/service/Dispatch.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Dispatch.h>
#include <commproto/service/ParserDelegatorFactory.h>
#include <commproto/service/ServiceChains.h>
#include <algorithm>
#include "TypeMapperImpl.h"

namespace commproto {
	namespace service {

		Connection::Connection(uint32_t id_, const commproto::sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTime_)
			: socket{ socket_ }
			, id{ id_ }
			, running{ false }
			, sleepMicro{ sleepTime_ }
			, dispatch{ dispatch_ }
			, builder{ std::make_shared<parser::MessageBuilder>(socket_,ParserDelegatorFactory::build(*this,dispatch_)) }
			, mapper{std::make_shared<messages::TypeMapperImpl>(std::make_shared<messages::TypeMapperObserver>(socket))}
			, channelMappingId(0)
		{
			socket->sendByte(sizeof(void*));
			channelMappingId = mapper->registerType<ChannelMappingMessage>();

			RegisterIdMessage registerId(mapper->registerType<RegisterIdMessage>(), id);
			socket->sendBytes(RegisterIdSerializer::serialize(std::move(registerId)));
		}

		Connection::~Connection()
		{
			stop();
			clearSubscriptions();
			dispatch->removeConnection(name);
		}

		void Connection::start()
		{
			if (running)
			{
				return;
			}
			running = true;
			worker = std::make_unique<std::thread>(&Connection::loop, this);
		}

		void Connection::stop()
		{
			if (!running)
			{
				return;
			}
			running = false;
			worker->join();
		}

		void Connection::send(const Message& msg)
		{
			messagesOut.enqueue(msg);
		}

		void Connection::receive(const Message& msg)
		{
			std::lock_guard<std::mutex> lock(subscriberMutex);
			for (ConnectionHandle con : subs)
			{
				con->send(msg);
			}
		}

		void Connection::setName(const std::string& name_)
		{
			name = name_;
		}

		void Connection::subscribe(const std::string& channelName)
		{
			LOG_INFO("Connection \"%s\" attempting to subscribe to \"%s\".",name.c_str(),channelName.c_str());

			if(channelName.compare("")==0)
			{
				//TODO: subscribe all
				return;
			}

			
			ConnectionHandle target = dispatch->getConnection(channelName);
			if (!target)
			{
				return;
			}
			ChannelMappingMessage mapping(channelMappingId, target->name, target->getId());
			send(ChannelMappingSerializer::serialize(std::move(mapping)));
			target->registerSubscription(shared_from_this());
		}

		void Connection::unsubscribe(const std::string& channelName)
		{
			LOG_INFO("Connection \"%s\" attempting to unsubscribe from \"%s\".", name.c_str(), channelName.c_str());
			ConnectionHandle target = dispatch->getConnection(channelName);
			if (!target)
			{
				//TODO: let the channel know there is no channel by that name
				return;
			}

			target->unregisterSubscription(shared_from_this());
		}

		void Connection::registerSubscription(const ConnectionHandle& subscriber)
		{
			LOG_INFO("Connection \"%s\" registered subscribtion from \"%s\".", name.c_str(), subscriber->name.c_str());
			std::lock_guard<std::mutex> lock(subscriberMutex);
			//TODO: send channel mapping
			if(std::find(subs.begin(), subs.end(), subscriber)!=subs.end())
			{
				return;
			}
			subs.emplace_back(subscriber);
		}

		void Connection::unregisterSubscription(const ConnectionHandle& subscriber)
		{
			LOG_INFO("Connection \"%s\" unregistered subscribtion from \"%s\".", name.c_str(), subscriber->name.c_str());
			std::lock_guard<std::mutex> lock(subscriberMutex);
			auto it = std::find(subs.begin(), subs.end(), subscriber);
			if (it != subs.end()) {
				subs.erase(it);
			}
		}

		void Connection::clearSubscriptions()
		{
			std::lock_guard<std::mutex> lock(subscriberMutex);
			subs.clear();
		}

		void Connection::loop()
		{
			LOG_INFO("Starting receive loop for connection with id %d", id);

			while (running && socket)
			{
				//recv part
				builder->pollAndRead();
				//send part
				Message msg;
				while (messagesOut.try_dequeue(msg))
				{
					int sent = socket->sendBytes(msg);
					if (sent != msg.size())
					{
						LOG_WARNING("Connection \"%s\"[%d] interrupted.", name.c_str(),id);
						stop();
					}

				}
				//std::this_thread::sleep_for(std::chrono::microseconds(sleepMicro));
			}
			LOG_INFO("Stopping receive loop for connection \"%s\"", name.c_str());
		}

		uint32_t Connection::getId() const
		{
			return id;
		}

		bool operator==(const Connection& lhs, const Connection& rhs)
		{
			return lhs.id == rhs.id && lhs.name.compare(rhs.name) == 0;
		}

	}
}
