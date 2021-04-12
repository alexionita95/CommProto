#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <string>
#include <rapidjson/document.h>

namespace commproto
{
    namespace JSONUtils
    {
		std::string stringify(const rapidjson::Document & doc);
		std::string stringify(const rapidjson::Value & obj);
    }
}


#endif //JSON_UTILS_H
