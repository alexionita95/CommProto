#ifndef SUBSCRIBE_HANDLER_H
#define SUBSCRIBE_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/service/Connection.h>

namespace commproto
{
	namespace service
	{
		class SubscribeHandler : public parser::Handler
		{
		public:
			SubscribeHandler(Connection& connection);
			void handle(messages::MessageBase&& data) override;
		private:
			Connection& connection;
		};
    }
}

#endif// SUBSCRIBE_HANDLER_H