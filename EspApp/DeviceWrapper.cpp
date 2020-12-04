#include "DeviceWrapper.h"

#include <Logging.h>
#include <commproto/variable/ContextImpl.h>
#include <commproto/parser/ParserDelegatorFactory.h>


DeviceWrapper::DeviceWrapper(SDKWrapper& device_, const char* host_, uint32_t port_)
	: client{ std::make_shared<commproto::sockets::SocketImpl>() }
	, host{ host_ }
	, port{ port_ }
	, device{ device_ }
	, state{ DeviceState::Searching }
{
}


class A
{
	
};

class B: public A
{
	
};


void DeviceWrapper::setup()
{
}
DeviceState DeviceWrapper::loop()
{
	switch (state)
	{
	case DeviceState::Searching:
	{
		LOG_INFO("Attempting to connect to %s:%d", host.c_str(), port);
		bool connected = client->initClient(host, port);
		if (!connected)
		{
			device.delay(5000);
		}
		LOG_INFO("Connection established to %s:%d", host.c_str(), port);
		state = DeviceState::Initializing;
	}
	break;
	case DeviceState::Initializing:
	{
		ctx = std::make_shared<variable::ContextImpl>(client);
		parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
		builder = std::make_shared<parser::MessageBuilder>(client, delegator);
		variable::VariableCallback cb = std::bind(&SDKWrapper::setLED,&device,std::placeholders::_1);
		ctx->subscribe("LED", cb);
		state = DeviceState::Connected;
		LOG_INFO("Initialized all necessary thingies, beginning communication");
	}
	break;

	case DeviceState::Connected:
	{
		builder->pollAndRead();
	}
	break;

	default:;
	}
	return state;
}
