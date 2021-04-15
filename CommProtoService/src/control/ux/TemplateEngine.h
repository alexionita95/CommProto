#ifndef TEMPLATE_ENGINE_H
#define TEMPLATE_ENGINE_H

#include <map>
#include <mutex>
#include <atomic>

namespace commproto
{
	namespace control 
	{
	
		namespace ux
		{
            class TemplateEngine
            {
            private:
                TemplateEngine();
                void initialize();

                std::map<std::string, std::string> templates;

            public:
                TemplateEngine(TemplateEngine const&) = delete;
                TemplateEngine& operator=(TemplateEngine const&) = delete;

                static std::shared_ptr<TemplateEngine> instance()
                {
                    static std::shared_ptr<TemplateEngine> s{ new TemplateEngine };
                    return s;
                }

                void loadTemplate(const std::string& name, const std::string& path);
                std::string getControlTemplate(const std::string& name, const std::map<std::string, std::string> values);

            };
            
		}
	}
}


#endif // TEMPLATE_ENGINE_H