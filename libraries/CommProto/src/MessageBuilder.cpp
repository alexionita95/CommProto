#include <commproto/parser/MessageBuilder.h>
#include <Logging.h>

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

		bool MessageBuilder::pollAndRead()
		{
            while(socket->pollSocket()){
				char byte = socket->readByte();
				switch (state)
				{
				case State::ReadingHandshake:
				{
					ptrSize = byte;
					state = State::ReadingPacketSize;
					expectedReadSize = sizeof(uint32_t);
					LOG_INFO("Read pointer size on client system: %d.", ptrSize);
				}
				break;
				case State::ReadingPacketSize:
				{
					if (internal.size() < expectedReadSize)
					{
						internal.push_back(byte);
					}
					if (internal.size() == expectedReadSize)
					{
						ByteStream stream(internal);
						stream.read(expectedReadSize);
						if (expectedReadSize <= 0)
						{
							LOG_ERROR("Possible error: 0 bytes read expected.");
							internal.clear();
							return false;
						}
						internal.clear();
						state = State::ReadingPayload;
						LOG_INFO("Expecting a message containing %d bytes.", expectedReadSize);
					}
				}
				break;
				case State::ReadingPayload:
				{
					if (internal.size() < expectedReadSize)
					{
						internal.push_back(byte);
					}
					if (internal.size() == expectedReadSize)
					{
						LOG_INFO("Finished reading a message containing %ld bytes.", internal.size());
						delegator->parse(internal);
						internal.clear();
						expectedReadSize = sizeof(uint32_t);
						state = State::ReadingPacketSize;
					}
				}
				break;
				}
            }
			return true;
		}

	}
}
