#include <SocketImpl.h>
#include <stdio.h>
#include <thread>
#include <Common.h>
#include <ParserDelegatorFactory.h>
#include <MessageBuilder.h>
#include "TypeMapper.h"
#include "../../SocketLib/src/TypeMapperImpl.h"

using namespace commproto;
Message makeMessage(const std::string& msg)
{
	Message msgBytes;
	msgBytes.reserve(msg.size());
	for (uint8_t c : msg) {
		msgBytes.push_back(c);
	}
	msgBytes.push_back(0);
	return msgBytes;
}

std::string makeString(const Message& msg)
{
	Message copy(msg);
	copy.push_back(0);
	return std::string(msg.data());
}


namespace apptest
{
	class TestData : public data::TypeBase
	{
	public:
		TestData(uint32_t type_)
			: TypeBase(type_)
		{
		}
		BP_TYPE_DEFAULTS(TestData);
		
	};

	
}
namespace commproto {
	namespace data {
		DEFINE_DATA_TYPE(apptest::TestData);
	}
}

int main(int argc, char*[])
{
	std::string stopMsg = "Die!";
	int port = 4242;
	std::string addr = "127.0.0.1";
	bool server = argc == 2;
	if (server) {

		socketlib::SocketHandle server = std::make_shared<socketlib::SocketImpl>();
		printf("Attempting to bind a socket to port %d...\n", port);
		bool init = server->initServer(port);
		if (!init) {
			printf("An error occurred while attempting to bind to port %d.\n", port);
			return 0;
		}
		printf("Succesfully bound to port %d. Awaiting next connection...\n", port);

		socketlib::SocketHandle client = server->acceptNext();

		if (client == nullptr) {
			printf("An error occurred while waiting for a connection.\n");
			return 0;
		}

		data::ParserDelegatorHandle delegator = data::ParserDelegatorFactory::build();
		data::MessageBuilderHandle builder = std::make_shared<data::MessageBuilder>(client, delegator);
		while(true)
		{
			builder->pollAndRead();
		}

	}
	else {
		socketlib::SocketHandle client = std::make_shared<socketlib::SocketImpl>();
		printf("Attempting to connect a client socket to %s:%d...\n", addr.c_str(), port);
		bool init = client->initClient(addr, port);
		if (!init) {
			printf("An error occurred while attempting to connect.\n");
			return 0;
		}

		//send ptr size
		Message msg;
		uint8_t sizeOfPtr = sizeof(void*);
;		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		data::TypeMapperObserverHandle observer = std::make_shared<data::TypeMapperObserver>(client);
		data::TypeMapperHandle mapper = std::make_shared<data::TypeMapperImpl>(observer);

		mapper->registerType<apptest::TestData>();
	}
	return 0;
}
