#include <commproto/service/RegisterChannelChain.h>



namespace commproto
{
	namespace service
	{
		Message RegisterChannelSerializer::serialize(messages::MessageBase&& data)
		{
			RegisterChannelMessage& message = static_cast<RegisterChannelMessage&>(data);
			parser::ByteStream stream;
			stream.writeHeader(data);
			stream.write(message.name);
			return stream.getStream();
		}

		void RegisterChannelParser::parse(parser::ByteStream&& msg)
		{
			std::string name;
			msg.skip(sizeof(uint32_t));
			msg.read(name);

			handler->handle(std::move(RegisterChannelMessage(0, name)));
		}

		RegisterChannelHandler::RegisterChannelHandler(Dispatch* dispatch_, const uint32_t id_)
			: dispatch{ dispatch_ }
			, id{ id_ }
		{
		}

		void RegisterChannelHandler::handle(messages::MessageBase&& data)
		{
			if(!dispatch)
			{
				return;
			}
			RegisterChannelMessage& message = static_cast<RegisterChannelMessage&>(data);
			dispatch->registerChannel(id,message.name);
		}
	}
}