#define PARSER_DELEGATOR_NO_WARNING

#include <commproto/service/Dispatch.h>
#include <SocketImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/service/Connection.h>
#include <commproto/config/ConfigParser.h>
#include <commproto/logger/FileLogger.h>
#include <termios.h>
#include <SerialInterface.h>

namespace ConfigValues
{
    static constexpr const char * const baudRate = "baud";
    static constexpr const int32_t defaultBaudRate = 115200;

    static constexpr const char * const device = "device";
    static constexpr const char * const defaultDevice = "/dev/ttyUSB0";

    static constexpr const char * const logToConsole = "logToConsole";
	static constexpr const bool logToConsoleDefault = true;
};


int main(int argc, const char * argv[])
{

    const char * configFile;
    if(argc <= 1)
    {
        configFile = "auth.cfg";
    }
    else
    {
        configFile = argv[1];
    }

    rapidjson::Document doc = commproto::config::ConfigParser(configFile).get();

    const int32_t baud = commproto::config::getValueOrDefault(doc, ConfigValues::baudRate, ConfigValues::defaultBaudRate);
	bool logToConsole = commproto::config::getValueOrDefault(doc, ConfigValues::logToConsole, ConfigValues::logToConsoleDefault);
    const char * const device = commproto::config::getValueOrDefault(doc, ConfigValues::device, ConfigValues::defaultDevice);


    commproto::logger::FileLogger logger("auth_log_" + commproto::logger::FileLogger::getTimestamp() + ".txt");
	if (!logToConsole)
	{
		logger.open();
		commproto::logger::setLoggable(&logger);
	}


    commproto::sockets::SocketHandle serial = std::make_shared<commproto::serial::SerialInterface>();

    LOG_INFO("Authentification service connecting to device %s on baudrate %d...",device,baud);
    if (!serial->initClient(device,B115200))
	{
        LOG_ERROR("A problem occurred while starting authentification service, shutting down...");
		return 1;
	}

    LOG_INFO("Authentification service started");

    while (true)
    {
        int poll = serial->pollSocket();
        for(uint32_t index = 0;index<poll;++index){
            int read = serial->readByte();
            if(read>=0){
                printf("%c",static_cast<char>(read));
            }
        }
	}
	return 0;
}
