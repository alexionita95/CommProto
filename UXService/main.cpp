#include <commproto/service/Dispatch.h>
#include <commproto/parser/ByteStream.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <sstream>
#include <conio.h>
#include <commproto/messages/SenderMaping.h>
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/MessageMapper.h>
#include <../CommProtoLib/src/parser/TypeMapperObserver.h>
#include <../CommProtoLib/src/parser/TypeMapperImpl.h>
#include <commproto/endpoint/ChannelParserDelegator.h>
#include <commproto/parser/ParserDelegatorUtils.h>
#include <commproto/endpoint/ParserDelegatorFactory.h>
#include <commproto/control/ParserDelegatorUtils.h>
#include <commproto/control/UIFactory.h>
#include <commproto/control/UxControllers.h>
#include "HTTPServer.h"
static int counter = 0;



using namespace commproto;
using namespace service;


parser::ParserDelegatorHandle buildSelfDelegator()
{
	std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<parser::ParserDelegator>();
	parser::buildBase(delegator);
	return delegator;
}

class UXServiceProvider : public endpoint::DelegatorProvider {
public:
	UXServiceProvider(const messages::TypeMapperHandle & mapper_, const sockets::SocketHandle & socket_, const control::ux::UxControllersHandle& controllers)
		: mapper{ mapper_ }
		, socket{ socket_ }
		, controllers{ controllers }
	{

	}
	parser::ParserDelegatorHandle provide(const std::string& name, const uint32_t id) override
	{
		parser::ParserDelegatorHandle delegator = buildSelfDelegator();
		auto controller = std::make_shared<control::ux::UIFactory>("UI", name, mapper, socket, id)->build();
		control::ux::addParsers(delegator, controller);
		controllers->addController(name, controller);

		LOG_INFO("Added controller for connection \"%s\" - %d", name.c_str(), id);

		return delegator;
	}
private:
	messages::TypeMapperHandle mapper;
	sockets::SocketHandle socket;
	control::ux::UxControllersHandle controllers;
};

void websiteLoop(int argc, char * argv[], const control::ux::UxControllersHandle & controllers)
{
	MyServerApp app(controllers);
	app.run(argc, argv);
}

int main(int argc, char * argv[])
{
	SenderMapping::InitializeName("Service::UX");
	sockets::SocketHandle socket = std::make_shared<sockets::SocketImpl>();
	if (!socket->initClient("localhost", 25565))
	{
		LOG_ERROR("A problem occurred while starting endpoint, shutting down...");
		return 0;
	}

	LOG_INFO("UX service started...");

	//send ptr size
	socket->sendByte(sizeof(void*));

	//core dependencies
	messages::TypeMapperObserverHandle observer = std::make_shared<messages::TypeMapperObserver>(socket);
	messages::TypeMapperHandle mapper = std::make_shared<messages::TypeMapperImpl>(observer);

	//registering our channel name
	uint32_t registerId = mapper->registerType<RegisterChannelMessage>();
	RegisterChannelMessage nameMsg(registerId, SenderMapping::getName());
	Message nameSerialized = RegisterChannelSerializer::serialize(std::move(nameMsg));
	socket->sendBytes(nameSerialized);

	//delegator to parse incoming messages
	control::ux::UxControllersHandle controllers = std::make_shared<control::ux::UxControllers>();
	std::shared_ptr<UXServiceProvider> provider = std::make_shared<UXServiceProvider>(mapper, socket, controllers);
	endpoint::ChannelParserDelegatorHandle channelDelegator = std::make_shared<endpoint::ChannelParserDelegator>(provider);
	parser::ParserDelegatorHandle delegator = endpoint::ParserDelegatorFactory::build(channelDelegator);
	channelDelegator->addDelegator(0, delegator);

	//subscribe - unsubscribe
	uint32_t registerSubId = mapper->registerType<SubscribeMessage>();
	uint32_t unsubId = mapper->registerType<UnsubscribeMessage >();
	uint32_t sendtoId = mapper->registerType<SendToMessage>();

	parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(socket, channelDelegator);

	//wait until we are sure we have an ID
	do
	{
		builder->pollAndRead();
	} while (SenderMapping::getId() == 0);


	SubscribeMessage sub(registerSubId, "");
	socket->sendBytes(SubscribeSerializer::serialize(std::move(sub)));

	std::thread websiteThread(websiteLoop, argc, argv, controllers);

	while (true) {
		builder->pollAndReadTimes(100u);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
