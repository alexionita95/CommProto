#include <commproto/service/ParserDelegator.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Connection.h>

namespace commproto
{
	namespace service
	{
		ParserDelegator::ParserDelegator(Connection& dispatch_)
			:owner{dispatch_}
		{
		}


		Message getSize(Message & msg)
		{
			uint32_t size = msg.size();
			parser::ByteStream stream;
			stream.write(size);
			return stream.getStream();
		}

		bool ParserDelegator::parse(Message& msg)
		{
			if(parser::ParserDelegator::parse(msg))
			{
				return true;
			}
			Message size = getSize(msg);
			msg.insert(msg.begin(),size.begin(),size.end());
			owner.receive(msg);
			return true;
		}
	}
}
