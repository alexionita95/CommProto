#include <commproto/service/Dispatch.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Connection.h>
#include <commproto/config/ConfigParser.h>
#include "commproto/logger/FileLogger.h"

struct ConfigValues
{
	static constexpr const char * const serverPort = "serverPort";
	static constexpr const uint32_t defaultServerPort = 25565;

	static constexpr const char * const logToConsole = "logToConsole";
	static constexpr const bool logToConsoleDefault = true;
};


int main(int argc, const char * argv[])
{
	rapidjson::Document doc = commproto::config::ConfigParser("dispatchConfig.cfg").get();

	const uint32_t port = commproto::config::getValueOrDefault(doc, ConfigValues::serverPort, ConfigValues::defaultServerPort);
	bool logToConsole = commproto::config::getValueOrDefault(doc, ConfigValues::logToConsole, ConfigValues::logToConsoleDefault);
	
	commproto::logger::FileLogger logger("dispatch_log_" + commproto::logger::FileLogger::getTimestamp() + ".txt");
	if (!logToConsole)
	{
		logger.open();
		commproto::logger::setLoggable(&logger);
	}

	commproto::service::Dispatch dsp;
	commproto::sockets::SocketHandle socket = std::make_shared<commproto::sockets::SocketImpl>();

	if (!socket->initServer(port))
	{
		LOG_ERROR("A problem occurred while starting dispatch service, shutting down...");
		return 1;
	}
	dsp.startCheckingConnections();

	LOG_INFO("Dispatch server started, wating for new connection...");

	while (true) {
		commproto::sockets::SocketHandle newCon = socket->acceptNext();
		dsp.addConnection(newCon);
	}
	return 0;
}
