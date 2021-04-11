#include <commproto/config/ConfigParser.h>
#include <fstream>
#include <sstream>


namespace commproto
{
    namespace config
    {
	    ConfigParser::ConfigParser(const std::string& fileName)
			: file {fileName}
	    {

	    }

	    rapidjson::Document ConfigParser::get() const
	    {
			std::ifstream stream;
			stream.open(file);
			rapidjson::Document doc;
			if(!stream.is_open())
			{
				return doc;
			}
			std::stringstream fileContents;
			std::string line;
			while(!stream.eof())
			{
				std::getline(stream, line);
				fileContents << line << std::endl;
			}
			std::string fileContentsString = fileContents.str();
			doc.Parse(fileContentsString.c_str(),fileContentsString.size());
			return doc;
	    }
    }
}
