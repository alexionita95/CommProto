#include <commproto/service/Dispatch.h>
#include <commproto/parser/ByteStream.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Connection.h>


int main(int argc, const char * argv[])
{
	commproto::service::Dispatch dsp;
	commproto::sockets::SocketHandle socket = std::make_shared<commproto::sockets::SocketImpl>();
	if (!socket->initServer(25565))
	{
		LOG_ERROR("A problem occurred while starting dispatch service, shutting down...");
		return 1;
	}
	LOG_INFO("Dispatch server started, wating for new connection...");

	while (true) {
		commproto::sockets::SocketHandle newCon = socket->acceptNext();
		int poll = 0;
		dsp.addConnection(newCon);
	}
	return 0;
}
