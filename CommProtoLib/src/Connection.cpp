#include <commproto/service/Dispatch.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Dispatch.h>
#include <commproto/service/ParserDelegatorFactory.h>

namespace commproto {
	namespace service {

		Connection::Connection(const std::string& name_, const commproto::sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTime_)
			: socket{ socket_ }
			, name{ name_ }
			, running{ false }
			, sleepTime{ sleepTime_ }
			, dispatch{ dispatch_ }
			, builder{std::make_shared<parser::MessageBuilder>(socket_,ParserDelegatorFactory::build(*this))}
		{
		}

		Connection::~Connection()
		{
			stop();
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
			//messagesIn.enqueue(msg);
			dispatch->sendAll(msg);
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
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			}
			LOG_INFO("Stopping receive loop for connection \"%s\"", name.c_str());
		}

	}
}