#ifndef COMMPROTO_PARSER_H
#define COMMPROTO_PARSER_H
#include <Handler.h>
#include <Common.h>
#include <ByteStream.h>

namespace commproto
{
	namespace data
	{
		class Parser
		{
		public:
			Parser(const HandlerHandle & handler_) : handler{ handler_ } {}
			virtual ~Parser() = default;

			virtual void parse(socketlib::ByteStream && msg) = 0;
		protected:
			HandlerHandle handler;
		};

		using ParserHandle = std::shared_ptr<Parser>;
	}
}

#endif