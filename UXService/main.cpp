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

using namespace commproto;
using namespace service;

MAKE_SINGLE_PROP_MESSAGE(StringMessage, std::string);

using StringParser = messages::SinglePropertyParser<std::string>;
using StringSerialize = messages::SinglePropetySerializer<std::string>;


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
		: stringId{0}
		, mapper{mapper_}
	{
		
	}
	parser::ParserDelegatorHandle provide(const std::string& name) override
	{
		parser::ParserDelegatorHandle delegator = buildSelfDelegator();

		stringId = mapper->registerType<StringMessage>();

		parser::HandlerHandle stringHandler = std::make_shared<StringHandler>();
		parser::ParserHandle stringParser = std::make_shared<StringParser>(stringHandler);
		delegator->registerParser<StringMessage>(stringParser);
		delegator->registerMapping(MessageName<StringMessage>::name(), stringId);

		return delegator;
	}
	uint32_t stringId;
private:
	messages::TypeMapperHandle mapper;
};


Message generateMessage(uint32_t id, int32_t attempt)
{
	std::stringstream writer;
	writer << "This is attempt #" << attempt << "!";
	uint32_t size = writer.str().size() + sizeof(uint32_t) + sizeof(uint32_t);
	StringMessage msg(id, writer.str());
	return StringSerialize::serialize(std::move(msg));
}

int main(int argc, const char * argv[])
{
	SenderMapping::InitializeName("Service::UX");
	sockets::SocketHandle socket = std::make_shared<sockets::SocketImpl>();
	if (!socket->initClient("localhost", 25565))
	{
		LOG_ERROR("A problem occurred while starting endpoint, shutting down...");
		return 1;
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
	std::shared_ptr<StringProvider> provider = std::make_shared<StringProvider>(mapper);
	endpoint::ChannelParserDelegatorHandle channelDelegator = std::make_shared<endpoint::ChannelParserDelegator>(provider);
	parser::ParserDelegatorHandle delegator = endpoint::ParserDelegatorFactory::build(channelDelegator);
	channelDelegator->addDelegator(0, delegator);

	//subscribe - unsubscribe
	uint32_t registerSubId = mapper->registerType<SubscribeMessage>();
	uint32_t unsubId = mapper->registerType<UnsubscribeMessage >();

	SubscribeMessage sub(registerSubId, "");

	socket->sendBytes(SubscribeSerializer::serialize(std::move(sub)));

	parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(socket, channelDelegator);

	//wait a bit for the messages to arrive
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	bool subscribed = true;
	while(true){
		builder->pollAndRead();
	}
	return 0;
}
