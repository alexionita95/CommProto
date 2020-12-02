#include <commproto/parser/ParserDelegator.h>
#include <Logging.h>

namespace commproto
{
	namespace parser
	{
		void ParserDelegator::registerMapping(const std::string& name, uint32_t id)
		{
			auto it = nameToParser.find(name);
			if (it == nameToParser.end())
			{
				return;
			}

			if (idToParser.find(id) != idToParser.end())
			{
				return;
			}
            LOG_DEBUG("Added a new parser mapping, %s - %d",name.c_str(), id);
			idToParser.emplace(id, it->second);
		}

		void ParserDelegator::parse(Message& msg)
		{
			ByteStream stream(msg);
			uint32_t msgId = 0;

			stream.read(msgId);
            LOG_DEBUG("Parsing a message with id %d",msgId);
			auto it = idToParser.find(msgId);
			if (it == idToParser.end())
			{
				return;
			}

			it->second->parse(std::move(stream));

		}

		void ParserDelegator::registerParserInternal(const std::string& typeName, const ParserHandle& parser)
		{
			auto it = nameToParser.find(typeName);

			if (it != nameToParser.end())
			{
				return;
			}
			nameToParser.emplace(typeName, parser);
		}
	}
}
