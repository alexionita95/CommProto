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
#include <Poco/Net/HTMLForm.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <commproto/control/UIController.h>
#include <commproto/control/ParserDelegatorUtils.h>
#include <commproto/control/UIFactory.h>
#include <commproto/control/UxControllers.h>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

static int counter = 0;

enum class ControlType : uint8_t
{
	Button = 0,
	Slider,
	Toggle,
	Label
};

const std::map<std::string, ControlType> stringMap = {
	{ "button",ControlType::Button },
	{ "slider",ControlType::Slider },
	{ "toggle",ControlType::Toggle },
	{ "label",ControlType::Label }
};

using KVMap = std::map<std::string, std::string>;

class MyRequestHandler : public HTTPRequestHandler
{
public:
	MyRequestHandler(const commproto::control::ux::UxControllersHandle & controllers) : controllers{ controllers }
	{

	}


	void handleBase(const KVMap & map, std::string & connection, uint32_t & controlId) const
	{
		auto conn = map.find("connection");
		if (conn != map.end())
		{
			connection = conn->second;
		}
		auto id = map.find("controlId");
		if (id != map.end())
		{
			try {
				controlId = std::stoi(id->second);
			}
			catch (std::invalid_argument arg)
			{
				//do nothing
			}
		}
	}

	void handleButton(KVMap && map)
	{
		std::string connection = "";
		uint32_t controlId = 0;
		handleBase(map, connection, controlId);
		auto controller = controllers->getController(connection);
		if(!controller)
		{
			return;
		}
		commproto::control::ux::ButtonHandle button = std::static_pointer_cast<commproto::control::ux::Button>(controller->getControl(controlId));

		if(!button)
		{
			return;
		}

		button->press();
	}

	void parseKVMap(KVMap&& map)
	{
		auto type = map.find("controlType");
		if (type == map.end())
		{
			return;
		}
		auto typeValue = stringMap.find(type->second);
		if(typeValue == stringMap.end())
		{
			return;
		}

		switch(typeValue->second)
		{
		case ControlType::Button: 
			handleButton(std::move(map));
			break;
		case ControlType::Slider: break;
		case ControlType::Toggle: break;
		case ControlType::Label: break;
		default: ;
		}

	}

	void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override
	{

		if (req.getMethod().compare("POST") == 0)
		{
			std::string url = req.getURI();
			if (url.compare("/update") == 0) {
				
				if (!controllers->hasUpdate())
				{
					resp.setStatusAndReason(HTTPResponse::HTTP_OK);
					resp.send();
					return;
				}
				ostream& out = resp.send();

				//TODO: actually handle other controllers
				auto simulator = controllers->getController("Endpoint::Simulator");
				if (simulator) {
					out << simulator->getUx();
				}
				resp.setStatus(HTTPResponse::HTTP_OK);
				out.flush();
				return;
			}
			if (url.compare("/action") == 0)
			{
				LOG_INFO("POST action - action ");
				string connection;
				string control;
				HTMLForm form(req, req.stream());
				KVMap map;
				for (NameValueCollection::ConstIterator i = form.begin(); i != form.end(); ++i) {
					map.emplace(i->first, i->second);
				}
				resp.setStatus(HTTPResponse::HTTP_OK);
				resp.send().flush();
				parseKVMap(std::move(map));
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
					out << line << std::endl;
				}
			}
			file.close();
			out.flush();
		}
	}

private:
	static int count;
	commproto::control::ux::UxControllersHandle controllers;

};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	MyRequestHandlerFactory(const commproto::control::ux::UxControllersHandle & controller) : controller{ controller } {}
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) override
	{
		return new MyRequestHandler(controller);
	}
private:
	commproto::control::ux::UxControllersHandle controller;
};

class MyServerApp : public ServerApplication
{
public:
	MyServerApp(const commproto::control::ux::UxControllersHandle & controller) : controller{ controller } {}
protected:

	int main(const vector<string> &)
	{
		HTTPServer s(new MyRequestHandlerFactory(controller), ServerSocket(9090), new HTTPServerParams);

		s.start();
		cout << endl << "Server started" << endl;

		waitForTerminationRequest();  // wait for CTRL-C or kill

		cout << endl << "Shutting down..." << endl;
		s.stop();

		return Application::EXIT_OK;
	}
private:
	commproto::control::ux::UxControllersHandle controller;
};

using namespace commproto;
using namespace service;


parser::ParserDelegatorHandle buildSelfDelegator()
{
	std::shared_ptr<parser::ParserDelegator> delegator = std::make_shared<parser::ParserDelegator>();
	parser::buildBase(delegator);
	return delegator;
}

struct Transferables
{
public:
	control::ux::UxControllersHandle controllers;
};

Transferables badPractice;

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

void websiteLoop(int argc, char * argv[])
{
	MyServerApp app(badPractice.controllers);
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
	badPractice.controllers = controllers;
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

	std::thread websiteThread(websiteLoop, argc, argv);

	while (true) {
		builder->pollAndReadTimes(100u);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
