#ifndef COMMPROTO_VARIABLE_MAPPING_PARSER_H
#define COMMPROTO_VARIABLE_MAPPING_PARSER_H

#include <commproto/variable/VariableMappingMessage.h>
#include <commproto/parser/Parser.h>

namespace commproto
{
	namespace variable
	{
		class VariableMappingParser : public parser::Parser
		{
		public:

			VariableMappingParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

		inline void VariableMappingParser::parse(parser::ByteStream&& msg)
		{
			std::string name;
			uint32_t index = 0;
			msg.read(name);
			msg.read(index);
			handler->handle(variable::VariableMappingMessage(0, name, index));
		}

	}
}

#endif //COMMPROTO_VARIABLE_PARSER_H