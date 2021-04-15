#include "HttpServer.h"
#include <commproto/logger/Logging.h>
#include <commproto/config/ConfigParser.h>

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&)
{
    return new UxRequestHandler(controller, mimetypes);
}

void RequestHandlerFactory::loadMimeTypes()
{

    LOG_INFO("Loading MIME Types...");
    rapidjson::Document file = commproto::config::ConfigParser("html_files/mime_types.json").get();
    for (rapidjson::Value::ConstMemberIterator itr = file.MemberBegin();
        itr != file.MemberEnd(); ++itr)
    {
        mimetypes.emplace(itr->name.GetString(), itr->value.GetString());
    }

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
