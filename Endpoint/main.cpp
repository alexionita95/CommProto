#include <commproto/service/Dispatch.h>
#include <commproto/parser/ByteStream.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <sstream>
#include <conio.h>
#include <commproto/messages/SenderMaping.h>
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/MessageMapper.h>
#include "../CommProtoLib/src/TypeMapperObserver.h"
#include "../CommProtoLib/src/TypeMapperImpl.h"

using namespace commproto;
using namespace service;

Message generateMessage(int32_t attempt)
{
	std::stringstream writer;
	parser::ByteStream stream;
	writer << "This is attempt #" << attempt << "!";
	uint32_t size = writer.str().size() + sizeof(uint32_t) + sizeof(uint32_t);
	LOG_INFO("Built message  \"%s\"(%d)", writer.str().c_str(), size);
	stream.write(size);
	stream.write((uint32_t)42);
	stream.write(writer.str());
	return stream.getStream();
}

int main(int argc, const char * argv[])
{
	SenderMapping::InitializeName("Endpoint::Simulator");
	sockets::SocketHandle socket = std::make_shared<sockets::SocketImpl>();
	if (!socket->initClient("localhost", 25565))
	{
		LOG_ERROR("A problem occurred while starting endpoint, shutting down...");
		return 1;
	}

	LOG_INFO("Endpoint started...");

	uint8_t attempt = 0;
	uint8_t maxAttempt = 10;
	socket->sendByte(sizeof(void*));
	messages::TypeMapperObserverHandle observer = std::make_shared<messages::TypeMapperObserver>(socket);
	messages::TypeMapperHandle mapper = std::make_shared<messages::TypeMapperImpl>(observer);

	uint32_t registerId = mapper->registerType<RegisterChannelMessage>();
	RegisterChannelMessage nameMsg(registerId, SenderMapping::getName());
	Message nameSerialized = RegisterChannelSerializer::serialize(std::move(nameMsg));
	socket->sendBytes(nameSerialized);

	for (uint32_t index = 0; index < maxAttempt; ++index)
	{
		int poll = 0;
		LOG_INFO("Sending attempt #%d", attempt);
		const int sent = socket->sendBytes(generateMessage(attempt++));
		LOG_INFO("Sent %d bytes", sent);
		do {
			poll = socket->pollSocket();
		} while (poll == 0);

		LOG_INFO("Got %d bytes back", sent);
		Message msg;
		socket->receive(msg, poll);
		parser::ByteStream reader(msg);
		std::string message;
		uint32_t placeholder;
		reader.read(placeholder);
		LOG_INFO("Got size = %d =? %d", sent, placeholder);
		reader.read(placeholder);
		reader.read(message);

		LOG_INFO("Got mesage: \"%s\"", message.c_str());

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}
	_getch();
	return 0;
}
