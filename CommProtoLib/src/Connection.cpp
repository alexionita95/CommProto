#include <commproto/service/Dispatch.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Dispatch.h>

namespace commproto {
	namespace service {

		Connection::Connection(const std::string& name_, const commproto::sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTime_)
			: socket{ socket_ }
			, name{ name_ }
			, running{ false }
			, sleepTime{ sleepTime_ }
			, dispatch{ dispatch_ }
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

		void Connection::loop()
		{
			LOG_INFO("Starting receive loop for connection \"%s\"", name.c_str());
			while (running && socket)
			{
				//recv part
				uint32_t poll = socket->pollSocket();
				if (poll > 0)
				{
					LOG_INFO("Got a message from connection \"%s\"", name.c_str());
					commproto::Message msg;
					int recv = socket->receive(msg, poll);
					if (recv != poll)
					{
						LOG_WARNING("Connection \"%s\" interrupted.", name.c_str());
						stop();
					}
					if (dispatch)
					{
						dispatch->sendAll(msg);
					}

					//messagesIn.push(msg);
				}
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