#ifndef COMMPROTO_HANDLER_H
#define COMMPROTO_HANDLER_H
#include <commproto/messages/MessageBase.h>
#include <memory>

namespace commproto
{
	namespace parser
	{
		class Handler
		{
		public:
			virtual ~Handler() = default;
			virtual void handle(messages::MessageBase && data) = 0;
		};
		using HandlerHandle = std::shared_ptr<Handler>;
	}

	
}

#endif