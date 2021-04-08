#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <rapidjson/document.h>

namespace commproto
{
    namespace config
    {
        class ConfigParser
        {
            public:
                ConfigParser(const std::string & fileName);
                rapidjson::Document get() const;
			private:
				std::string file;
		};



		template <typename T>
		T getValueOrDefault(rapidjson::Document & doc, const char* const name, const T& defaultValue)
		{
			return defaultValue;
		}

		template <>
		inline bool getValueOrDefault(rapidjson::Document & doc, const char* const name, const bool& defaultValue)
		{
            if(doc.HasParseError() || !doc.IsObject())
            {
                return defaultValue;
            }
            if (!doc.HasMember(name))
            {
                return defaultValue;
            }
            if (!doc[name].IsBool())
			{
				return defaultValue;
			}
			return doc[name].GetBool();
		}

		template <>
		inline int32_t getValueOrDefault(rapidjson::Document & doc, const char* const name, const int32_t& defaultValue)
        {
            if(doc.HasParseError() || !doc.IsObject())
            {
                return defaultValue;
            }

            if (!doc.HasMember(name))
			{
				return defaultValue;
			}

            if(!doc[name].IsInt())
            {
                return defaultValue;
            }

			return doc[name].GetInt();
		}

	    inline const char* getValueOrDefault(rapidjson::Document & doc, const char* const name, const char * defaultValue)
		{
            if(doc.HasParseError() || !doc.IsObject())
            {
                return defaultValue;
            }
            if (!doc.HasMember(name))
            {
                return defaultValue;
            }
            if (!doc[name].IsString())
			{
				return defaultValue;
			}
			return doc[name].GetString();
		}
    }
}


#endif // CONFIG_PARSER_H
