#include <commproto/service/Dispatch.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/messages/SenderMaping.h>
#include <commproto/service/ServiceChains.h>
#include <commproto/messages/MessageMapper.h>
#include <../CommProtoLib/src/parser/TypeMapperObserver.h>
#include <../CommProtoLib/src/parser/TypeMapperImpl.h>
#include <commproto/endpoint/ChannelParserDelegator.h>
#include <commproto/endpoint/ParserDelegatorFactory.h>
#include <commproto/control/UxControllers.h>
#include "HTTPServer.h"
#include "UxDelegatorProvider.h"
#include "commproto/endpoint/ChannelMappingHandler.h"

using namespace commproto;

void websiteLoop(int argc, char * argv[], const commproto::control::ux::UxControllersHandle & controllers)
{
	UxServerApp app(controllers);
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
	messages::TypeMapperHandle mapper = messages::TypeMapperFactory::build(socket);

	//registering our channel name
	uint32_t registerId = mapper->registerType<service::RegisterChannelMessage>();
	service::RegisterChannelMessage nameMsg(registerId, SenderMapping::getName());
	Message nameSerialized = service::RegisterChannelSerializer::serialize(std::move(nameMsg));
	socket->sendBytes(nameSerialized);

	//delegator to parse incoming messages
	control::ux::UxControllersHandle controllers = std::make_shared<control::ux::UxControllers>();
	std::shared_ptr<UXServiceProvider> provider = std::make_shared<UXServiceProvider>(mapper, socket, controllers);
	endpoint::ChannelParserDelegatorHandle channelDelegator = std::make_shared<endpoint::ChannelParserDelegator>(provider);

	endpoint::MappingNotification removeController = [controllers](const std::string & name, const uint32_t id)
	{
		LOG_INFO("Removing UX controller for connection %s(%d)",name.c_str(),id);
		controllers->removeController(name);
	};
	channelDelegator->subscribeToChannelRemoval(removeController);
	parser::ParserDelegatorHandle delegator = endpoint::ParserDelegatorFactory::build(channelDelegator);
	channelDelegator->addDelegator(0, delegator);

	//subscribe - unsubscribe
	uint32_t registerSubId = mapper->registerType<service::SubscribeMessage>();
	uint32_t unsubId = mapper->registerType<service::UnsubscribeMessage >();
	uint32_t sendtoId = mapper->registerType<service::SendToMessage>();

	parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(socket, channelDelegator);

	//wait until we are sure we have an ID
	do
	{
		builder->pollAndRead();
	} while (SenderMapping::getId() == 0);


	service::SubscribeMessage sub(registerSubId, "");
	socket->sendBytes(service::SubscribeSerializer::serialize(std::move(sub)));

	std::thread websiteThread(websiteLoop, argc, argv, controllers);

	while (true) {
		builder->pollAndReadTimes(100u);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
