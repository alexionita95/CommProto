#ifndef COMMPROTO_SERVICE_HANDLERS_H
#define COMMPROTO_SERVICE_HANDLERS_H

//TODO: CLEANUP - Split into multiple files

#include <commproto/endpoint/ChannelParserDelegator.h>
#include <commproto/parser/Handler.h>
#include <commproto/service/Dispatch.h>

namespace commproto
{
	namespace service
	{
		class Connection;

		class RegisterChannelHandler : public parser::Handler
		{
		public:
			RegisterChannelHandler(Dispatch* dispatch, const uint32_t id);
			void handle(messages::MessageBase&& data) override;
		private:
			Dispatch* dispatch;
			const uint32_t id;
		};

		class SubscribeHandler : public parser::Handler
		{
		public:
			SubscribeHandler(Connection& connection);
			void handle(messages::MessageBase&& data) override;
		private:
			Connection& connection;
		};


		class UnsubscribeHandler : public parser::Handler
		{
		public:
			UnsubscribeHandler(Connection& connection);
			void handle(messages::MessageBase&& data) override;
		private:
			Connection& connection;
		};


		class RegisterIdHandler : public parser::Handler
		{
		public:
			void handle(messages::MessageBase&& data) override;
		};


		class MappingHandler : public parser::Handler
		{
		public:
			MappingHandler(const endpoint::ChannelParserDelegatorHandle & delegator);
			void handle(messages::MessageBase&& data) override;
		private:
			endpoint::ChannelParserDelegatorHandle delegator;
		};

	}
}

#endif// COMMPROTO_SERVICE_HANDLERS_H