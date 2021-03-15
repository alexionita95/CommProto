#ifndef COMMPROTO_PARSER_DELEGATOR_H
#define COMMPROTO_PARSER_DELEGATOR_H

#include <map>

#include <commproto/parser/Parser.h>
#include <commproto/parser/ByteStream.h>
#include <commproto/messages/MessageName.h>

namespace commproto
{
	namespace parser
	{
		class ParserDelegator
		{
		public:
			virtual ~ParserDelegator() = default;
			template <class T>
			void registerParser(const ParserHandle & parser);

			void registerMapping(const std::string & name, uint32_t id);

			virtual bool parse(Message & msg);

		private:

			void registerParserInternal(const std::string & typeName, const ParserHandle & parser);
			std::map<std::string, ParserHandle> nameToParser;
			std::map<uint32_t, ParserHandle> idToParser;
		};

		template <class T>
		void ParserDelegator::registerParser(const ParserHandle& parser)
		{
			registerParserInternal(messages::MessageName<T>::name(),parser);
		}


		using ParserDelegatorHandle = std::shared_ptr<ParserDelegator>;
	}
}

#endif