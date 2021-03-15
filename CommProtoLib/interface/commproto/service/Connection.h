#ifndef COMMPROTO_SERVICE_CONNECTION_H
#define COMMPROTO_SERVICE_CONNECTION_H
#include <thread>
#include <commproto/sockets/Socket.h>
#include <commproto/parser/MessageBuilder.h>
#include <atomic>
#include <concurrentqueue.h>


namespace commproto
{
	namespace service
	{

		class Dispatch;
		using DispatchHandle = Dispatch *;


		class Connection
		{
		public:

			Connection(const std::string& name_, const sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTime_ = 10);
			~Connection();
			void start();
			void stop();
			void loop();
			void send(const Message& msg);
			void receive(const Message& msg);

		private:

			sockets::SocketHandle socket;
			std::string name;
			std::unique_ptr<std::thread> worker;
			moodycamel::ConcurrentQueue<Message> messagesOut;
			moodycamel::ConcurrentQueue<Message> messagesIn;
			std::atomic_bool running;
			const uint32_t sleepTime;
			DispatchHandle dispatch;
			parser::MessageBuilderHandle builder;
		};


		using ConnectionHandle = std::shared_ptr<Connection>;
	}
}

#endif //COMMPROTO_SERVICE_CONNECTION_H