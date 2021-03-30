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
#include <commproto/parser/ParserDelegatorFactory.h>
#include <commproto/parser/ParserDelegatorUtils.h>
#include <commproto/endpoint/ParserDelegatorFactory.h>
#include <commproto/control/UIFactory.h>


using namespace commproto;
using namespace service;

MAKE_SINGLE_PROP_MESSAGE(StringMessage, std::string);

using StringParser = messages::SinglePropertyParser<std::string>;
using StringSerialize = messages::SinglePropertySerializer<std::string>;


namespace commproto
{
	DEFINE_DATA_TYPE(StringMessage);
}

class StringHandler : public parser::Handler
{
public:
	void handle(messages::MessageBase&& data) override;
};

void StringHandler::handle(messages::MessageBase&& data)
{
	StringMessage& message = static_cast<StringMessage&>(data);
	LOG_INFO("%s [sender:%d]", message.prop.c_str(), message.senderId);
}

parser::ParserDelegatorHandle buildSelfDelegator()
{
	std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<parser::ParserDelegator>();
	parser::buildBase(delegator);
	return delegator;
}


class StringProvider : public endpoint::DelegatorProvider{
public:
	StringProvider(const messages::TypeMapperHandle & mapper_)
		: mapper{mapper_}
	{
		
	}
	parser::ParserDelegatorHandle provide(const std::string& name) override
	{
		parser::ParserDelegatorHandle delegator = buildSelfDelegator();
		parser::HandlerHandle stringHandler = std::make_shared<StringHandler>();
		parser::ParserHandle stringParser = std::make_shared<StringParser>(stringHandler);
		delegator->registerParser<StringMessage>(stringParser);
		return delegator;
	}
private:
	messages::TypeMapperHandle mapper;
};

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
	uint8_t maxAttempt = 1;

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
	std::shared_ptr<StringProvider> provider = std::make_shared<StringProvider>(mapper);
	endpoint::ChannelParserDelegatorHandle channelDelegator = std::make_shared<endpoint::ChannelParserDelegator>(provider);
	parser::ParserDelegatorHandle delegator = endpoint::ParserDelegatorFactory::build(channelDelegator);
	channelDelegator->addDelegator(0, delegator);

	parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(socket, channelDelegator);

	//wait until we are sure we have an ID
	do
	{
		builder->pollAndRead();
	} while (SenderMapping::getId() == 0);

	for(uint32_t index = 0; index<100;++index)
	{
		builder->pollAndRead();
	}

	while (true)
	{
		builder->pollAndRead();
	}
	socket->close();
	_getch();
	return 0;
}
