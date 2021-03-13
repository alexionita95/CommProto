#include <commproto/service/Dispatch.h>
#include <commproto/parser/ByteStream.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>

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

	std::string name;


	while (true) {
		commproto::sockets::SocketHandle newCon = socket->acceptNext();

		int poll = 0;

		do {
			poll = newCon->pollSocket();
		} while (poll == 0);
		
		commproto::Message nameMSg;
		newCon->receive(nameMSg, poll);
		commproto::parser::ByteStream stream(nameMSg);
		stream.read(name);

		dsp.addConnection(name, newCon);
		LOG_INFO("A new connection has been added: \"%s\"", name.c_str());
	}
	return 0;
}
