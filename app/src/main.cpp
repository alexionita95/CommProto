#include <SocketImpl.h>
#include <stdio.h>
#include <thread>
#include <Common.h>
#include <ParserDelegatorFactory.h>
#include <MessageBuilder.h>
#include "TypeMapper.h"
#include "../../SocketLib/src/TypeMapperImpl.h"
#include "Variable.h"

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

void printValue(data::VariableBaseHandle & var)
{
	switch (var->getType())
	{
	case data::ValueType::integer32:

		printf("Got an integer value: %d\n", std::static_pointer_cast<data::IntegerVariable>(var)->get());

		break;
	case data::ValueType::string:
		printf("Got a string value: %s\n", std::static_pointer_cast<data::StringVariable>(var)->get().c_str());
		break;
	case data::ValueType::real32:
		printf("Got a real value: %f\n", std::static_pointer_cast<data::RealVariable>(var)->get());
		break;
	default:;
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

		if (!client) {
			printf("An error occurred while waiting for a connection.\n");
			return 0;
		}

		data::ContextHandle ctx = std::make_shared<data::ContextImpl>(client);

		data::VariableCallback cb = &printValue;
		ctx->subscribe(0, cb);
		ctx->subscribe(1, cb);

		data::ParserDelegatorHandle delegator = data::ParserDelegatorFactory::build(ctx);
		data::MessageBuilderHandle builder = std::make_shared<data::MessageBuilder>(client, delegator);
		while (true)
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
		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		data::TypeMapperObserverHandle observer = std::make_shared<data::TypeMapperObserver>(client);
		data::TypeMapperHandle mapper = std::make_shared<data::TypeMapperImpl>(observer);

		//mapper->registerType<apptest::TestData>();

		data::ContextHandle ctx = std::make_shared<data::ContextImpl>(client, mapper->registerType<data::VariableMessage>());

		auto var = std::make_shared<data::IntegerVariable>(ctx, 42);
		var->setIndex(ctx->registerOutVariable(var));
		*var = 3;

		auto var2 = std::make_shared<data::StringVariable>(ctx, "yooo");
		var2->setIndex(ctx->registerOutVariable(var2));

		*var2 = "sup";


	}
	return 0;
}
