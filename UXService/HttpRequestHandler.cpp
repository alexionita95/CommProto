#include "HttpRequestHandler.h"
#include <commproto/control/Button.h>
#include <commproto/logger/Logging.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <fstream>
#include <commproto/control/Toggle.h>
#include <Poco/Path.h>
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include "commproto/utils/JSONUtils.h"

const std::map<std::string, ControlType> stringMap = {
	{ "button",ControlType::Button },
	{ "slider",ControlType::Slider },
	{ "toggle",ControlType::Toggle },
	{ "label",ControlType::Label }
};

void UxRequestHandler::handleBase(const KVMap& map, std::string& connection, uint32_t& controlId) const
{
	auto conn = map.find("connection");
	if (conn != map.end())
	{
		connection = conn->second;
	}
	auto id = map.find("controlId");
	if (id != map.end())
	{
		try
		{
			controlId = std::stoi(id->second);
		}
		catch (std::invalid_argument arg)
		{
			//do nothing
		}
	}
}

void UxRequestHandler::handleButton(KVMap&& map) const
{
	std::string connection = "";
	uint32_t controlId = 0;
	handleBase(map, connection, controlId);
	auto controller = controllers->getController(connection);
	if (!controller)
	{
		return;
	}
	commproto::control::ux::ButtonHandle button = std::static_pointer_cast<commproto::control::ux::Button>(controller->getControl(controlId));

	if (!button)
	{
		return;
	}

	button->press();
}

void UxRequestHandler::parseKVMap(KVMap&& map) const
{
	auto type = map.find("controlType");
	if (type == map.end())
	{
		return;
	}
	auto typeValue = stringMap.find(type->second);
	if (typeValue == stringMap.end())
	{
		return;
	}

	switch (typeValue->second)
	{
	case ControlType::Button:
		handleButton(std::move(map));
		break;
	case ControlType::Slider: break;
	case ControlType::Toggle:
		handleToggle(std::move(map));
		break;
	case ControlType::Label: break;
	default: ;
	}
}

void UxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
	if (req.getMethod().compare("POST") == 0)
	{
		std::string url = req.getURI();

		if(url.find("/notification") == 0)
		{
			bool update = url.find("force") != std::string::npos;
			auto ctrls = controllers->getControllers();

			if(!update)
			{
				for(auto it = ctrls.begin(); it != ctrls.end(); ++it)
				{
					if(it->second->hasNotifications())
					{
						update = true;
						break;
					}
				}
			}

			if(!update)
			{
				resp.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
				resp.send() << "<null>";
				return;
			}

			resp.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
			std::ostream& out = resp.send();
			resp.setContentType("application/json");

			rapidjson::Document doc;
			auto & alloc = doc.GetAllocator();
			doc.SetArray();

			for (auto it = ctrls.begin(); it != ctrls.end(); ++it)
			{
				if(it->second->hasNotifications())
				{
					rapidjson::Document notif;
					notif.Parse(it->second->getNotifications().c_str());
					if (notif.IsArray())
					{
						doc.PushBack(notif.GetArray(), alloc);
					}
				}
			}
			std::string temp = commproto::JSONUtils::stringify(doc);
			out << temp;


		}

		if (url.find("/update") == 0)
		{
			bool update = controllers->hasUpdate() || (url.find("force")!=std::string::npos);
			if (!update)
			{
				resp.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
				resp.send() << "<null>";
				return;
			}
			std::ostream& out = resp.send();

			//TODO: actually handle other controllers
			auto simulator = controllers->getController("Endpoint::Simulator");
			if (simulator)
			{
				out << simulator->getUx();
			}
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
			out.flush();
			return;
		}
		if (url.compare("/action") == 0)
		{
			LOG_INFO("POST action - action ");
			std::string connection;
			std::string control;
			Poco::Net::HTMLForm form(req, req.stream());
			KVMap map;
			for (Poco::Net::NameValueCollection::ConstIterator i = form.begin(); i != form.end(); ++i)
			{
				map.emplace(i->first, i->second);
			}
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
			resp.send().flush();
			parseKVMap(std::move(map));
		}
	}
	if(req.getMethod().compare("GET") == 0)
	{
		std::string uri = req.getURI();
		if(uri == "/")
		{
			uri = "/index.html";
		}

		LOG_INFO("GET request for file \"%s\"", uri.c_str());

		Poco::Path path(uri);
		std::string extension = Poco::toLower(path.getExtension());
		if (extension == "css") {
			resp.setContentType("text/css");
		} 
		else if(extension == "html")
		{
			resp.setContentType("text/html");
		} 
		else if(extension == "js")
		{
			resp.setContentType("application/javascript");
		}
		else
		{
			resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			return;
		}
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

		Poco::Path root("html_files");
		std::string reqFile = uri.substr(1);
		root.append(reqFile);
		std::ostream& out = resp.send();
		std::ifstream file(root.toString());
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::string line;
				std::getline(file, line);
				out << line << std::endl;
			}
		}
		file.close();
		out.flush();
	}
}

void UxRequestHandler::handleToggle(KVMap&& map) const
{
	std::string connection = "";
	uint32_t controlId = 0;
	handleBase(map, connection, controlId);
	auto controller = controllers->getController(connection);
	if (!controller)
	{
		return;
	}
	commproto::control::ux::ToggleHandle toggle = std::static_pointer_cast<commproto::control::ux::Toggle>(controller->getControl(controlId));

	if (!toggle)
	{
		return;
	}

	toggle->toggle();
}
