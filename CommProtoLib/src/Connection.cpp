#include <commproto/service/Dispatch.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Dispatch.h>
#include <commproto/service/ParserDelegatorFactory.h>

namespace commproto {
	namespace service {

		Connection::Connection(uint32_t id_, const commproto::sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTime_)
			: socket{ socket_ }
			, id{id_}
			, running{ false }
			, sleepMicro{ sleepTime_ }
			, dispatch{ dispatch_ }
			, builder{std::make_shared<parser::MessageBuilder>(socket_,ParserDelegatorFactory::build(*this))}
		{
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
			LOG_INFO("Enqueued message to connection \"%s\"", name.c_str());
			messagesOut.enqueue(msg);
		}

		void Connection::receive(const Message& msg)
		{
			std::lock_guard<std::mutex> lock(subscriberMutex);
			for(ConnectionHandle con : subs)
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
			ConnectionHandle target =  dispatch->getConnection(channelName);
			if(!target)
			{
				//TODO: let the channel know there is no channel by that name
				return;
			}

			target->registerSubscription(shared_from_this());
		}

		void Connection::unsubscribe(const std::string& channelName)
		{
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
			std::lock_guard<std::mutex> lock(subscriberMutex);
			subs.emplace_back(subscriber);
		}

		void Connection::unregisterSubscription(const ConnectionHandle& subscriber)
		{
			std::lock_guard<std::mutex> lock(subscriberMutex);
			auto it = subs.end();

			for(auto it2 = subs.begin(); it2 != subs.end(); ++it2)
			{
				if(it2->get() == it->get())
				{
					it = it2;
					break;
				}
			}
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
			LOG_INFO("Starting receive loop for connection \"%s\"", name.c_str());
			while (running && socket)
			{
				//recv part
				builder->pollAndRead();
				//send part
				Message msg; 
				while (messagesOut.try_dequeue(msg))
				{
					LOG_INFO("Sent a message to connection \"%s\"", name.c_str());
					int sent = socket->sendBytes(msg);
					if (sent != msg.size())
					{
						LOG_WARNING("Connection \"%s\" interrupted.", name.c_str());
						stop();
					}

				}
				std::this_thread::sleep_for(std::chrono::microseconds(sleepMicro));
			}
			LOG_INFO("Stopping receive loop for connection \"%s\"", name.c_str());
		}

	}
}