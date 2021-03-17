#ifndef COMMPROTO_REGISTER_CHANNEL_CHAIN_H
#define COMMPROTO_REGISTER_CHANNEL_CHAIN_H

#include <commproto/parser/Parser.h>
#include <commproto/parser/Handler.h>
#include <commproto/service/RegisterChannelMessage.h>
#include <commproto/common/Common.h>
#include <commproto/parser/ByteStream.h>
#include <commproto/service/Dispatch.h>

namespace commproto
{
	namespace service
	{

		class RegisterChannelSerializer
		{
		public:
			static Message serialize(messages::MessageBase&& data);
		};

		class RegisterChannelParser : public parser::Parser
		{
		public:
			RegisterChannelParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

		class RegisterChannelHandler : public parser::Handler
		{
		public:
			RegisterChannelHandler(Dispatch* dispatch, const uint32_t id);
			void handle(messages::MessageBase&& data) override;
		private:
			Dispatch* dispatch;
			const uint32_t id;
		};
	}
}

#endif// COMMPROTO_REGISTER_CHANNEL_CHAIN_H