#ifndef COMMPROTO_PARSER_H
#define COMMPROTO_PARSER_H
#include <commproto/parser/Handler.h>
#include <commproto/parser/ByteStream.h>


namespace commproto
{
	namespace parser
	{
		class Parser
		{
		public:
			Parser(const HandlerHandle & handler_) : handler{ handler_ } {}
			virtual ~Parser() = default;

			virtual void parse(parser::ByteStream && msg) = 0;
		protected:
			HandlerHandle handler;
		};

		using ParserHandle = std::shared_ptr<Parser>;
	}
}

#endif