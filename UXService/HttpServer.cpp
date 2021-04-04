#include "HTTPServer.h"

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
	return new UxRequestHandler(controller);
}

int UxServerApp::main(const std::vector<std::string>&)
{
	Poco::Net::HTTPServer s(new RequestHandlerFactory(controller), Poco::Net::ServerSocket(port), new Poco::Net::HTTPServerParams);

	s.start();
	std::cout << std::endl << "Server started" << std::endl;

	waitForTerminationRequest(); // wait for CTRL-C or kill

	std::cout << std::endl << "Shutting down..." << std::endl;
	s.stop();

	return Application::EXIT_OK;
}
