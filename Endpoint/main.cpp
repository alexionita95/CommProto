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
#include "commproto/service/ParserDelegatorFactory.h"

using namespace commproto;
using namespace service;

MAKE_SINGLE_PROP_MESSAGE(StringMessage, std::string);

using StringParser = messages::SinglePropertyParser<std::string>;
using StringSerialize = messages::SinglePropetySerializer<std::string>;


namespace commproto
{
	namespace messages
	{
		DEFINE_DATA_TYPE(StringMessage);
	}
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

	uint32_t stringId = mapper->registerType<StringMessage>();
	parser::ParserDelegatorHandle delegator = endpoint::ParserDelegatorFactory::build();
	parser::HandlerHandle stringHandler = std::make_shared<StringHandler>();
	parser::ParserHandle stringParser = std::make_shared<StringParser>(stringHandler);
	delegator->registerParser<StringMessage>(stringParser);
	delegator->registerMapping(messages::MessageName<StringMessage>::name(), stringId);

	uint32_t registerSubId = mapper->registerType<SubscribeMessage>();
	uint32_t unsubId = mapper->registerType<UnsubscribeMessage >();
	UnsubscribeMessage unsub(unsubId, SenderMapping::getName());
	SubscribeMessage sub(registerSubId, SenderMapping::getName());
	socket->sendBytes(SubscribeSerializer::serialize(std::move(sub)));

	parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(socket, delegator);

	bool subscribed = true;
	for (uint32_t index = 0; index < maxAttempt; ++index)
	{
		int poll = 0;
		LOG_INFO("Sending attempt #%d", attempt);
		const int sent = socket->sendBytes(generateMessage(stringId, attempt++));
		do {
			poll = socket->pollSocket();
		} while ( subscribed && poll == 0);

		builder->pollAndRead();
		if(subscribed && attempt>=maxAttempt/2)
		{
			LOG_INFO("unsubscribing...");
			subscribed = false;
			socket->sendBytes(UnsubscribeSerializer::serialize(std::move(unsub)));
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

	}
	_getch();
	return 0;
}
