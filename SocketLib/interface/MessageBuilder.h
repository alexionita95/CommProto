#ifndef COMMPROTO_MESSAGE_BUILDER_H
#define COMMPROTO_MESSAGE_BUILDER_H
#include <Common.h>
#include <Socket.h>
#include <ByteStream.h>
#include <ParserDelegator.h>

namespace commproto
{
	namespace data
	{
		enum class State : uint8_t
		{
			ReadingHandshake,
			ReadingPacketSize,
			ReadingPayload,
		};

		class MessageBuilder
		{
		public:
			MessageBuilder(const socketlib::SocketHandle& socket_, const ParserDelegatorHandle& delegator_);

			void pollAndRead();
			static uint8_t ptrSize;
		private:
			socketlib::SocketHandle socket;
			State state;
			Message internal;
			uint32_t expectedReadSize;
			ParserDelegatorHandle delegator;

		};

		using MessageBuilderHandle = std::shared_ptr<MessageBuilder>;
	}
}


#endif