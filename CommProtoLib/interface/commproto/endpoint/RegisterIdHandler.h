#ifndef REGISTER_ID_HANDLER_H
#define REGISTER_ID_HANDLER_H

#include <commproto/parser/Handler.h>

namespace commproto
{
	namespace endpoint
	{
		class RegisterIdHandler : public parser::Handler
		{
		public:
			void handle(messages::MessageBase&& data) override;
		};

	}
}

#endif// REGISTER_ID_HANDLER_H