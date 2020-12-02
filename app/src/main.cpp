#include <SocketImpl.h>
#include <stdio.h>
#include <thread>
#include <commproto/common/Common.h>
#include <commproto/parser/ParserDelegatorFactory.h>
#include <commproto/parser/MessageBuilder.h>
#include <commproto/messages/MessageMapper.h>
#include "../../CommProto/src/TypeMapperImpl.h"
#include <commproto/variable/Variable.h>
#include <commproto/variable/ContextImpl.h>
#include <Logging.h>

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


void printValue(variable::VariableBaseHandle & var)
{
	switch (var->getType())
	{
	case variable::ValueType::integer32:
		LOG_INFO("Got an integer value: %d", std::static_pointer_cast<variable::IntegerVariable>(var)->get());
		break;
	case variable::ValueType::string:
		LOG_INFO("Got a string value: %s", std::static_pointer_cast<variable::StringVariable>(var)->get().c_str());
		break;
	case variable::ValueType::real32:
		LOG_INFO("Got a real value: %f", std::static_pointer_cast<variable::RealVariable>(var)->get());
		break;
	case variable::ValueType::bool8:
		LOG_INFO("Got a boolean value: %s", std::static_pointer_cast<variable::BoolVariable>(var)->get()?"True":"False");
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

		sockets::SocketHandle server = std::make_shared<sockets::SocketImpl>();
		LOG_INFO("Attempting to bind a socket to port %d...", port);
		bool init = server->initServer(port);
		if (!init) {
			LOG_ERROR("An error occurred while attempting to bind to port %d.", port);
			return 0;
		}
		LOG_INFO("Succesfully bound to port %d. Awaiting next connection...", port);


		while (true) {
			sockets::SocketHandle client = server->acceptNext();

			if (!client) {
				printf("An error occurred while waiting for a connection.");
				continue;
			}

			variable::ContextHandle ctx = std::make_shared<variable::ContextImpl>(client);

			variable::VariableCallback cb = &printValue;
			ctx->subscribe(0, cb);
			ctx->subscribe(1, cb);
			ctx->subscribe(2, cb);

			parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
			parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(client, delegator);
			bool connected = true;
			do
			{
				connected = builder->pollAndRead();
			} while (connected);
		}

	}
	else {
		sockets::SocketHandle client = std::make_shared<sockets::SocketImpl>();
		LOG_INFO("Attempting to connect a client socket to %s:%d...", addr.c_str(), port);
		bool init = client->initClient(addr, port);
		if (!init) {
			LOG_ERROR("An error occurred while attempting to connect.");
			return 0;
		}

		//send ptr size
		Message msg;
		uint8_t sizeOfPtr = sizeof(void*);
		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		messages::TypeMapperObserverHandle observer = std::make_shared<messages::TypeMapperObserver>(client);
		messages::TypeMapperHandle mapper = std::make_shared<messages::TypeMapperImpl>(observer);

		variable::ContextHandle ctx = std::make_shared<variable::ContextImpl>(client, mapper->registerType<variable::VariableMessage>());

		auto var = std::make_shared<variable::IntegerVariable>(ctx, 42);
		ctx->registerOutVariable(var);
		*var = 3;

		auto var2 = std::make_shared<variable::StringVariable>(ctx, "yooo");
		ctx->registerOutVariable(var2);

		*var2 = "sup";

		auto var3 = std::make_shared<variable::BoolVariable>(ctx, true);
		ctx->registerOutVariable(var3);

		*var3 = false;

	}
	return 0;
}
