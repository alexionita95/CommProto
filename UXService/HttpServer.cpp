#include "HttpServer.h"
#include <commproto/logger/Logging.h>

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
	return new UxRequestHandler(controller);
}

int UxServerApp::main(const std::vector<std::string>&)
{
	Poco::Net::HTTPServer s(new RequestHandlerFactory(controller), Poco::Net::ServerSocket(port), new Poco::Net::HTTPServerParams);

	s.start();
	LOG_INFO("HTTP Server started");

	waitForTerminationRequest(); // wait for CTRL-C or kill

	LOG_INFO("HTTP Server shutting down");
	s.stop();

	return Application::EXIT_OK;
}
