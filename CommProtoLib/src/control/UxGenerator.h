#ifndef UX_GENERATOR_H
#define UX_GENERATOR_H

#include <commproto/control/UIController.h>
#include "ButtonImpl.h"
#include <sstream>
#include "ToggleImpl.h"
#include "LabelImpl.h"
#include "NotificationImpl.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace commproto
{
	namespace control
	{
		namespace ux
		{
			class Generator
			{
			public:
				Generator(UIController& manager_)
					: manager{ manager_ }
				{

				}
				template <typename ControlType>
				std::string generate(const ControlType& control) const;

				void send(Message && msg) const;
				void notifyUpdate() const;
			protected:
				UIController& manager;

			};

			inline void Generator::send(Message&& msg) const
			{
				manager.send(msg);
			}

			inline void Generator::notifyUpdate() const
			{
				manager.notifyUpdate();
			}

			template <typename ControlType>
			std::string Generator::generate(const ControlType& control) const
			{
				return "";
			}

			template <>
			inline std::string Generator::generate(const ButtonImpl& control) const
			{
				std::stringstream sstream;
				sstream << "<button onclick = \"postButton('" << manager.getConnectionName() << "','" << control.getId() << "')\">" << control.getName() << " </button>";
				return sstream.str();
			}

			template <>
			inline std::string Generator::generate(const ToggleImpl& control) const
			{
				std::stringstream sstream;
				sstream << manager.getConnectionName() << "-toggle" << control.getId();
				std::string controlIdString = sstream.str();
				sstream.clear();
				sstream.str(std::string());

				sstream << "<div class=\"toggle-switch\">";
				sstream << control.getName() << " : <input type=\"checkbox\" id=\""<< controlIdString <<"\" onclick=\"postToggle(this, '"<< manager.getConnectionName() <<"', '"<< control.getId() <<"')\"" << (control.getState()?" checked>":" >");
				sstream << "<label for=\""<< controlIdString <<"\"></label>";
				sstream << "</div>";
				return sstream.str();
			}
			 
			template <>
			inline std::string Generator::generate(const LabelImpl& control) const
			{
				std::stringstream sstream;
				sstream <<"<span class=\"c_label\">" << control.getName() << ": "<< control.getText() << "</span>";
				return sstream.str();
			}


			template <>
			inline std::string Generator::generate(const NotificationImpl& control) const
			{
				
				rapidjson::Document obj;
				auto &alloc = obj.GetAllocator();

				rapidjson::Value connectionId(std::to_string(manager.getConnectionId()).c_str(), alloc);
				obj.AddMember("connection", connectionId, alloc);

				rapidjson::Value notifId(std::to_string(control.getId()).c_str(),alloc);
				obj.AddMember("id", notifId, alloc);

				std::vector<std::string> options = control.getOptions();
				rapidjson::Value arr;
				arr.SetArray();
				for(auto str: options)
				{
					arr.PushBack(rapidjson::Value{}.SetString(str.c_str(),str.length()), alloc);
				}

				obj.AddMember("options", arr, alloc);

				rapidjson::StringBuffer sb;
				rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
				obj.Accept(writer);

				return sb.GetString();


			}


			using GeneratorHandle = std::shared_ptr<Generator>;
		}
	}
}

#endif //UX_GENERATOR_H