#include <commproto/service/Dispatch.h>
#include <commproto/parser/ByteStream.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <sstream>
#include <conio.h>
commproto::Message generateMessage(int32_t attempt)
{
	std::stringstream writer;
	commproto::parser::ByteStream stream;
	writer << "This is attempt #" << attempt << "!";
	LOG_INFO("Built message: \"%s\"", writer.str().c_str());
	stream.write(writer.str());
	return stream.getStream();
}

int main(int argc, const char * argv[])
{
	commproto::sockets::SocketHandle socket = std::make_shared<commproto::sockets::SocketImpl>();
	if (!socket->initClient("localhost", 25565))
	{
		LOG_ERROR("A problem occurred while starting endpoint, shutting down...");
		return 1;
	}

	LOG_INFO("Endpoint started...");

	std::string name = "testconnection";
	commproto::parser::ByteStream writer;
	writer.write(name);

	int32_t sent = socket->sendBytes(writer.getStream());
	LOG_INFO("Started a connection named \"%s\"", name.c_str());
	LOG_INFO("Msg size : %d =? %d", writer.getStream().size(), sent);
	uint8_t attempt = 0;
	uint8_t maxAttempt = 10;



	for (uint32_t index = 0; index < maxAttempt; ++index)
	{
		int poll = 0;
		LOG_INFO("Sending attempt #%d", attempt);
		const int sent = socket->sendBytes(generateMessage(attempt++));
		LOG_INFO("Sent %d bytes",sent);
		do {
			poll = socket->pollSocket();
		} while (poll == 0);

		LOG_INFO("Got %d bytes back", sent);
		commproto::Message msg;
		socket->receive(msg, poll);
		commproto::parser::ByteStream reader(msg);
		std::string message;
		reader.read(message);

		LOG_INFO("Got mesage: \"%s\"", message.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}
	_getch();
	return 0;
}
