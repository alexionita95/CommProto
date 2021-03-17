#ifndef COMMPROTO_SERVICE_CONNECTION_H
#define COMMPROTO_SERVICE_CONNECTION_H
#include <thread>
#include <commproto/sockets/Socket.h>
#include <commproto/parser/MessageBuilder.h>
#include <atomic>
#include <concurrentqueue.h>
#include <mutex>


namespace commproto
{
	namespace service
	{

		class Dispatch;
		using DispatchHandle = Dispatch *;
		class Connection;
		using ConnectionHandle = std::shared_ptr<Connection>;
		using Subscribers = std::vector<ConnectionHandle>;


		class Connection : public std::enable_shared_from_this<Connection>
		{
		public:

			Connection(uint32_t id, const sockets::SocketHandle& socket_, const DispatchHandle& dispatch_, uint32_t sleepTimeMicro_ = 10);
			~Connection();
			void start();
			void stop();
			void loop();
			void send(const Message& msg);
			void receive(const Message& msg);
			void setName(const std::string & name);

			void subscribe(const std::string & channelName);
			void unsubscribe(const std::string & channelName);

			void registerSubscription(const ConnectionHandle& subscriber);
			void unregisterSubscription(const ConnectionHandle& subscriber);
		
			void clearSubscriptions();
			friend bool operator==(const Connection& lhs, const Connection& rhs);
		private:

			sockets::SocketHandle socket;
			std::string name;
			const uint32_t id;
			std::unique_ptr<std::thread> worker;
			moodycamel::ConcurrentQueue<Message> messagesOut;
			std::atomic_bool running;
			const uint32_t sleepMicro;
			DispatchHandle dispatch;
			parser::MessageBuilderHandle builder;
			Subscribers subs;
			std::mutex subscriberMutex;
		};

		inline bool operator==(const Connection& lhs, const Connection& rhs)
		{
			return lhs.id == rhs.id && lhs.name.compare(rhs.name) == 0;
		}
	}
}

#endif //COMMPROTO_SERVICE_CONNECTION_H