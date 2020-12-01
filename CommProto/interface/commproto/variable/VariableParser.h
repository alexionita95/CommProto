#ifndef COMMPROTO_VARIABLE_PARSER_H
#define COMMPROTO_VARIABLE_PARSER_H

#include <commproto/variable/Variable.h>
#include <commproto/parser/Parser.h>

namespace commproto
{
	namespace variable
	{
		class VariableParser : public parser::Parser
		{
		public:

			VariableParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			template <typename T>
			void parse(parser::ByteStream&& msg, const uint32_t index, const ValueType type);
			void parse(parser::ByteStream&& msg) override;
		};

		inline void VariableParser::parse(parser::ByteStream&& msg)
		{
			uint8_t valueType = 0;
			uint32_t index = 0;
			msg.read(valueType);
			msg.read(index);
			ValueType type = static_cast<ValueType>(valueType);

			switch (type)
			{
			case ValueType::integer32:
				parse<uint32_t>(std::move(msg), index, type);
				break;
			case ValueType::string:
				parse<std::string>(std::move(msg), index, type);
				break;
			case ValueType::real32:
				parse<float>(std::move(msg), index, type);
				break;
			default: ;
			}
		}

		template <typename T>
		void VariableParser::parse(parser::ByteStream&& msg, const uint32_t index, const ValueType type)
		{
			T value{};
			msg.read(value);
			handler->handle(std::move(VariableMessageTemplated<T>(0, type, index, value)));
		}
	}
}

#endif //COMMPROTO_VARIABLE_PARSER_H