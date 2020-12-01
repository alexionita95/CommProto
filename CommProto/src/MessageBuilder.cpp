#include <commproto/parser/MessageBuilder.h>


namespace commproto
{
	namespace parser
	{

		uint8_t MessageBuilder::ptrSize = sizeof(void*);

		MessageBuilder::MessageBuilder(const sockets::SocketHandle& socket_, const ParserDelegatorHandle& delegator_)
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
				printf("Read pointer size on client system: %d.\n", ptrSize);
			}
			break;
			case State::ReadingPacketSize:
			{
				if (internal.size() <= expectedReadSize)
				{
					printf("*");
					internal.push_back(socket->readByte());
				}
				if (internal.size() == expectedReadSize)
				{
					ByteStream stream(internal);
					stream.read(expectedReadSize);
					if (expectedReadSize == 0)
					{
						printf("\n[========]Possible error: 0 bytes read expected.\n");
						internal.clear();
						return;
					}
					internal.clear();
					state = State::ReadingPayload;
					printf("\n[====]Expecting a message containing %d bytes.\n", expectedReadSize);
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
					printf("Finished reading a message containing %ld bytes.\n", internal.size());
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
