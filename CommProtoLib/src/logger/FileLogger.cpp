#include <commproto/logger/FileLogger.h>
#include <iomanip>
#include <sstream>

namespace commproto
{
	namespace logger
	{
		FileLogger::FileLogger(const std::string& fileName_, const bool alwaysFlush_)
			: alwaysFlush{ alwaysFlush_ }
			, fileName{ fileName_ }
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
			std::stringstream stream;
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);
			stream << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
			return  stream.str();
		}

		void FileLogger::open()
		{
			file.open(fileName, std::ofstream::app | std::ofstream::out);
		}
	}
}
