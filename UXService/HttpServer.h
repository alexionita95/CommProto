#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <commproto/control/UxControllers.h>
#include "HttpRequestHandler.h"
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServer.h>


class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	MyRequestHandlerFactory(const commproto::control::ux::UxControllersHandle & controller) : controller{ controller } {}
	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest&) override;
private:
	commproto::control::ux::UxControllersHandle controller;
};

class MyServerApp : public Poco::Util::ServerApplication
{
public:
	MyServerApp(const commproto::control::ux::UxControllersHandle & controller, const uint32_t port = 9090) : controller{ controller }, port{ port } {}
protected:
	int main(const std::vector<std::string>&) override;
private:
	commproto::control::ux::UxControllersHandle controller;
	const uint32_t port;
};


#endif // HTTP_SERVER_H