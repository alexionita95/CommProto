#include <commproto/service/Dispatch.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Connection.h>
#include <commproto/config/ConfigParser.h>

const uint32_t defaultPort = 25565;

int main(int argc, const char * argv[])
{
	commproto::service::Dispatch dsp;
	std::atomic_bool checkAlive = true;
	commproto::sockets::SocketHandle socket = std::make_shared<commproto::sockets::SocketImpl>();


	uint32_t port = defaultPort;
	rapidjson::Document doc = commproto::config::ConfigParser("dispatchConfig.cfg").get();
	if (!doc.HasParseError() && doc.HasMember("port") && doc["port"].IsInt())
	{
		port = doc["port"].GetInt();
		LOG_INFO("Found port in config file = %d", port);
	}
	else
	{
		LOG_WARNING("Config file not found or does not contain port, starting with default port = %d", port);
	}



	if (!socket->initServer(port))
	{
		LOG_ERROR("A problem occurred while starting dispatch service, shutting down...");
		return 1;
	}
	LOG_INFO("Dispatch server started, wating for new connection...");

	std::thread checkAliveThread([&dsp, &checkAlive]() {
		while (checkAlive) {
			dsp.checkActiveConnections();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});

	while (true) {
		commproto::sockets::SocketHandle newCon = socket->acceptNext();
		dsp.addConnection(newCon);
	}
	checkAlive = false;
	checkAliveThread.join();
	return 0;
}
