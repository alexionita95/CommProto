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



#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

static int counter = 0;

class MyRequestHandler : public HTTPRequestHandler
{
public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
		if(req.getMethod().compare("POST")==0)
		{
			std::string url = req.getURI();
			if (url.compare("/update") == 0) {
				ostream& out = resp.send();
				out << "update #" << counter++ << " :D <br>";
				out.flush();
			}
		}
		else {
			resp.setStatus(HTTPResponse::HTTP_OK);
			resp.setContentType("text/html");

			ostream& out = resp.send();
			ifstream file("cp\\index.html");
			if (file.is_open())
			{
				while (!file.eof()) {
					std::string line;
					std::getline(file, line);
					out << line;
				}
			}
			file.close();
			out.flush();
		}
	}

private:
	static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
	{
		return new MyRequestHandler;
	}
};

class MyServerApp : public ServerApplication
{
protected:
	int main(const vector<string> &)
	{
		HTTPServer s(new MyRequestHandlerFactory, ServerSocket(9090), new HTTPServerParams);

		s.start();
		cout << endl << "Server started" << endl;

		waitForTerminationRequest();  // wait for CTRL-C or kill

		cout << endl << "Shutting down..." << endl;
		s.stop();

		return Application::EXIT_OK;
	}
};

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


Message generateMessage(uint32_t id, int32_t attempt)
{
	std::stringstream writer;
	writer << "This is attempt #" << attempt << "!";
	uint32_t size = writer.str().size() + sizeof(uint32_t) + sizeof(uint32_t);
	StringMessage msg(id, writer.str());
	return StringSerialize::serialize(std::move(msg));
}


void serviceApp()
{
	SenderMapping::InitializeName("Service::UX");
	sockets::SocketHandle socket = std::make_shared<sockets::SocketImpl>();
	if (!socket->initClient("localhost", 25565))
	{
		LOG_ERROR("A problem occurred while starting endpoint, shutting down...");
		return;
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

	//wait until we are sure we have an ID
	do
	{
		builder->pollAndRead();
	} while (SenderMapping::getId() == 0);

	bool subscribed = true;
	while (true) {
		builder->pollAndRead();
	}
	return;
}


int main(int argc, char * argv[])
{

	MyServerApp app;
	std::thread appThread(serviceApp);
	int res =  app.run(argc, argv);
	appThread.join();
	return res;
}
