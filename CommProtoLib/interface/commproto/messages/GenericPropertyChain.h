#ifndef COMMPROTO_PROPERTY_CHAIN_H
#define COMMPROTO_PROPERTY_CHAIN_H

#include <commproto/parser/Parser.h>
#include <commproto/messages/GenericPropertyMessage.h>
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


		template <typename  T, typename  U>
		class DoublePropertySerializer
		{
		public:
			static Message serialize(messages::MessageBase&& data);
		};

		template <typename T, typename  U>
		Message DoublePropertySerializer<T, U>::serialize(messages::MessageBase&& data)
		{
			DoublePropertyMessage<T, U>& realData = static_cast<DoublePropertySerializer<T, U>&>(data);
			parser::ByteStream stream;
			stream.writeHeader(data);
			stream.write<T>(realData.prop);
			stream.write<U>(realData.prop2);
			return stream.getStream();
		}


		template <typename  T, typename  U, typename V>
		class TriplePropertySerializer
		{
		public:
			static Message serialize(messages::MessageBase&& data);
		};

		template <typename  T, typename  U, typename V>
		Message TriplePropertySerializer<T, U, V>::serialize(messages::MessageBase&& data)
		{
			TriplePropertyMessage<T, U, V>& realData = static_cast<TriplePropertyMessage<T, U, V>&>(data);
			parser::ByteStream stream;
			stream.writeHeader(data);
			stream.write<T>(realData.prop);
			stream.write<U>(realData.prop2);
			stream.write<V>(realData.prop3);
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
			uint32_t  type = 0, sender = 0;
			T prop;
			stream.read<uint32_t>(type);
			stream.read < uint32_t>(sender);
			stream.read<T>(prop);
			SinglePropertyMessage<T> realMessage(type, prop);
			realMessage.senderId = sender;
			handler->handle(std::move(realMessage));
		}


		template <typename  T, typename U>
		class DoublePropertyParser : public parser::Parser
		{
		public:
			DoublePropertyParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

		template <typename  T, typename U>
		void DoublePropertyParser<T, U>::parse(parser::ByteStream&& msg)
		{

			parser::ByteStream stream(msg);
			uint32_t  type = 0, sender = 0;
			T prop;
			U prop2;
			stream.read<uint32_t>(type);
			stream.read < uint32_t>(sender);
			stream.read<T>(prop);
			stream.read<U>(prop2);
			DoublePropertyMessage<T, U> realMessage(type, prop, prop2);
			realMessage.senderId = sender;
			handler->handle(std::move(realMessage));
		}

		template <typename  T, typename U, typename V>
		class TriplePropertyParser : public parser::Parser
		{
		public:
			TriplePropertyParser(const parser::HandlerHandle& handler_)
				: Parser(handler_)
			{
			}

			void parse(parser::ByteStream&& msg) override;
		};

		template <typename  T, typename U, typename V>
		void TriplePropertyParser<T, U, V>::parse(parser::ByteStream&& msg)
		{

			parser::ByteStream stream(msg);
			uint32_t  type = 0, sender = 0;
			T prop;
			U prop2;
			V prop3;
			stream.read<uint32_t>(type);
			stream.read < uint32_t>(sender);
			stream.read<T>(prop);
			stream.read<U>(prop2);
			stream.read<V>(prop3);
			TriplePropertyMessage<T, U, V> realMessage(type, prop, prop2, prop3);
			realMessage.senderId = sender;
			handler->handle(std::move(realMessage));
		}
	}
}

#endif// COMMPROTO_PROPERTY_CHAIN_H