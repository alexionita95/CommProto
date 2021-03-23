#ifndef SINGLE_PROPERTY_CHAIN_H
#define SINGLE_PROPERTY_CHAIN_H

#include <commproto/parser/Parser.h>
#include <commproto/messages/SinglePropertyMessage.h>
#include <commproto/common/Common.h>
#include <commproto/parser/ByteStream.h>
#include <commproto/service/Dispatch.h>

namespace commproto
{
	namespace messages
	{
		template <typename  T>
		class SinglePropetySerializer
		{
		public:
			static Message serialize(messages::MessageBase&& data);
		};

		template <typename T>
		Message SinglePropetySerializer<T>::serialize(messages::MessageBase&& data)
		{
			SinglePropertyMessage<T>& realData = static_cast<SinglePropertyMessage<T>&>(data);
			parser::ByteStream stream;
			stream.writeHeader(data);
			stream.write<T>(realData.prop);
			return stream.getStream();
		}


		template <typename  T>
		class SinglePropertyParser : public parser::Parser
		{
		public:
			SinglePropertyParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

		template <typename T>
		void SinglePropertyParser<T>::parse(parser::ByteStream&& msg)
		{

			parser::ByteStream stream(msg);
			uint32_t sender = 0;
			T prop;
			stream.read < uint32_t>(sender);
			stream.read<T>(prop);
			SinglePropertyMessage<T> realMessage(0, prop);
			realMessage.senderId = sender;
			handler->handle(std::move(realMessage));
		}

	}
}

#endif// SINGLE_PROPERTY_CHAIN_H