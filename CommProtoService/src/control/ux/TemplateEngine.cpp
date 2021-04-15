#include "TemplateEngine.h"
#include <sstream>
#include <fstream>
#include <commproto/logger/Logging.h>
#include <regex>

namespace commproto
{
	namespace control {
	
		namespace ux
		{
            TemplateEngine::TemplateEngine()
            {
                initialize();
            }

            void TemplateEngine::initialize()
            {
                loadTemplate("notification", "html_files/templates/notification.html");
            }

            void TemplateEngine::loadTemplate(const std::string & name, const std::string & path)
            {
                LOG_INFO("Loadeding template %s from path %s", name.c_str(), path.c_str());
                std::ifstream templateFile(path);
                std::stringstream fileStream;

                if (templateFile.is_open())
                {
                    while (!templateFile.eof())
                    {
                        std::string line;
                        std::getline(templateFile, line);
                        fileStream << line << std::endl;
                    }
                }
                std::string htmlTemplate = fileStream.str();
                fileStream.clear();
                templateFile.close();
                if (templates.find(name) == templates.end()) 
                {
                    LOG_INFO("Loaded template %s from path %s", name.c_str(), path.c_str());
                    templates.emplace(name, htmlTemplate);
                }
            }
            std::string TemplateEngine::getControlTemplate(const std::string & name, const std::map<std::string, std::string> values)
            {
                if (templates.find(name) == templates.end())
                    return std::string();

                std::string htmlTemplate = templates[name];
                for (auto& value : values)
                {
                    std::string regex = "\\@";
                    regex.append(value.first);
                    htmlTemplate = std::regex_replace(htmlTemplate, std::regex(regex), value.second);
                }

                return htmlTemplate;
            }
        }
	}
}
