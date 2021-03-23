#ifndef UNSUBSCRIBE_HANDLER_H
#define UNSUBSCRIBE_HANDLER_H

#include <commproto/parser/Handler.h>
#include <commproto/service/Connection.h>

namespace commproto
{
    namespace service
    {
		class UnsubscribeHandler : public parser::Handler
		{
		public:
			UnsubscribeHandler(Connection& connection);
			void handle(messages::MessageBase&& data) override;
		private:
			Connection& connection;
		};

    }
}

#endif// UNSUBSCRIBE_HANDLER_H