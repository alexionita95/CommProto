#include <commproto/utils/JSONUtils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace commproto
{
    namespace JSONUtils
    {
	    std::string stringify(const rapidjson::Document& doc)
	    {
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
			doc.Accept(writer);
			return std::string{ sb.GetString() };
	    }

	    std::string stringify(const rapidjson::Value& obj)
	    {
			rapidjson::StringBuffer sb;
			rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
			obj.Accept(writer);
			return std::string{ sb.GetString() };
	    }
    }
}
