#ifndef COMMPROTO_HANDLER_H
#define COMMPROTO_HANDLER_H
#include <TypeBase.h>
#include <memory>

namespace commproto
{
	namespace data
	{
		class Handler
		{
		public:
			virtual ~Handler() = default;
			virtual void handle(TypeBase && data) = 0;
		};
		using HandlerHandle = std::shared_ptr<Handler>;
	}

	
}

#endif