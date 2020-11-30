#ifndef COMMPROTO_PARSER_DELEGATOR_H
#define COMMPROTO_PARSER_DELEGATOR_H
#include <Parser.h>
#include <map>
#include <ByteStream.h>
#include <TypeName.h>
#include <stdio.h>

namespace commproto
{
	namespace data
	{
		class ParserDelegator
		{
		public:
			template <class T>
			void registerParser(const ParserHandle & parser);

			void registerMapping(const std::string & name, uint32_t id);

			void parse(Message & msg);

		private:
			std::map<std::string, ParserHandle> nameToParser;
			std::map<uint32_t, ParserHandle> idToParser;
		};

		using ParserDelegatorHandle = std::shared_ptr<ParserDelegator>;

		inline void ParserDelegator::registerMapping(const std::string& name, uint32_t id)
		{
			auto it = nameToParser.find(name);
			if (it == nameToParser.end())
			{
				printf("Could not find a parser for type: %s(id:%d).\n", name.c_str(),id);
				return;
			}

			if(idToParser.find(id) != idToParser.end())
			{
				printf("Attempted to register a mapping for %s(id:%d), but one already exists.\n", name.c_str(), id);
				return;
			}
			printf("Found and registered a parser mapping for type: %s(id:%d).\n", name.c_str(), id);
			idToParser.emplace(id, it->second);
		}

		inline void ParserDelegator::parse(Message& msg)
		{
			socketlib::ByteStream stream(msg);
			uint32_t msgId = 0;

			stream.read(msgId);

			auto it = idToParser.find(msgId);
			if(it == idToParser.end())
			{
				printf("Could not parse messaage with id %d.\n",msgId);
				return;
			}

			it->second->parse(std::move(stream));

		}

		template <class T>
		void ParserDelegator::registerParser(const ParserHandle& parser)
		{
			std::string typeName = TypeName<T>::name();
			auto it = nameToParser.find(typeName);
			
			if(it!=nameToParser.end())
			{
				return;
			}
			
			nameToParser.emplace(typeName, parser);
		}

	}
}

#endif