#include <commproto/logger/FileLogger.h>
#include <iomanip>
#include <sstream>

namespace commproto
{
	namespace logger
	{
		FileLogger::FileLogger(const std::string& fileName_, const bool alwaysFlush_)
            : fileName{ fileName_ }
            , alwaysFlush{ alwaysFlush_ }
		{
		}

		bool FileLogger::isOpen() const
		{
			return  file.is_open();
		}

		void FileLogger::addLog(const char* line, const uint32_t size)
		{
			if (!file.is_open())
			{
				return;
			}
			file.write(line, size);
			if (alwaysFlush)
			{
				file.flush();
			}
		}

		void FileLogger::close()
		{
			if (file.is_open())
			{
				file.flush();
				file.close();
			}
		}

		FileLogger::~FileLogger()
		{
			FileLogger::close();
		}

		std::string FileLogger::getTimestamp()
		{
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[80];

            time (&rawtime);
            timeinfo = localtime(&rawtime);

            strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
            std::string str(buffer);

            return str;
		}

		void FileLogger::open()
		{
			file.open(fileName, std::ofstream::app | std::ofstream::out);
		}
	}
}
