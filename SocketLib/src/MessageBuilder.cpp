#include <MessageBuilder.h>


namespace commproto
{
	namespace data
	{

		uint8_t MessageBuilder::ptrSize = sizeof(void*);

		MessageBuilder::MessageBuilder(const socketlib::SocketHandle& socket_, const ParserDelegatorHandle& delegator_)
			: socket{ socket_ }
			, state{ State::ReadingHandshake }
			, expectedReadSize{ 0 }
			, delegator{ delegator_ }
		{
		}

		void MessageBuilder::pollAndRead()
		{
			switch (state)
			{
			case State::ReadingHandshake:
			{
				char byte = socket->readByte();
				ptrSize = byte;
				state = State::ReadingPacketSize;
				expectedReadSize = sizeof(uint32_t);
			}
			break;
			case State::ReadingPacketSize:
			{
				if (internal.size() <= expectedReadSize)
				{
					internal.push_back(socket->readByte());
				}
				if (internal.size() == expectedReadSize)
				{
					socketlib::ByteStream stream(internal);
					stream.read(expectedReadSize);
					internal.clear();
					state = State::ReadingPayload;
				}
			}
			break;
			case State::ReadingPayload:
			{
				if (internal.size() <= expectedReadSize)
				{
					internal.push_back(socket->readByte());
				}
				if (internal.size() == expectedReadSize)
				{
					delegator->parse(internal);
					internal.clear();
					expectedReadSize = sizeof(uint32_t);
					state = State::ReadingPacketSize;
				}
			}
			break;
			}

		}

	}
}
