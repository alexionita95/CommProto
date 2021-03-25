#ifndef KEEP_ALIVE_CHAIN_H
#define KEEP_ALIVE_CHAIN_H

#include <commproto/messages/MessageBase.h>
#include <commproto/messages/MessageName.h>
#include <commproto/parser/Parser.h>

namespace commproto
{
    namespace messages
    {
        class KeepAliveMessage : public MessageBase
        {
        public:
			KeepAliveMessage(const uint32_t &type) : MessageBase(type)
			{
				
			}
			BP_TYPE_DEFAULTS(KeepAliveMessage);


        };

		class KeepAliveParser : public parser::Parser
		{
		public:
			explicit KeepAliveParser(const parser::HandlerHandle& handler_ = nullptr)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

	    inline void KeepAliveParser::parse(parser::ByteStream&& msg)
	    {
			// does nothing
	    }

		class KeepAliveSerializer
	    {
	    public:
			static Message serialize(messages::MessageBase && data)
			{
				parser::ByteStream stream;
				stream.writeHeader(data);
				return stream.getStream();
			}
	    };


    }
	DEFINE_DATA_TYPE(messages::KeepAliveMessage);
}



#endif//KEEP_ALIVE_CHAIN_H
