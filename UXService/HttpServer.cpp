#include "HTTPServer.h"

Poco::Net::HTTPRequestHandler* MyRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
	return new MyRequestHandler(controller);
}

int MyServerApp::main(const std::vector<std::string>&)
{
	Poco::Net::HTTPServer s(new MyRequestHandlerFactory(controller), Poco::Net::ServerSocket(port), new Poco::Net::HTTPServerParams);

	s.start();
	std::cout << std::endl << "Server started" << std::endl;

	waitForTerminationRequest(); // wait for CTRL-C or kill

	std::cout << std::endl << "Shutting down..." << std::endl;
	s.stop();

	return Application::EXIT_OK;
}
