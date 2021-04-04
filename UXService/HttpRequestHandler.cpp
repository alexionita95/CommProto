#include "HttpRequestHandler.h"
#include <commproto/control/Button.h>
#include <commproto/logger/Logging.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <fstream>

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

void UxRequestHandler::handleButton(KVMap&& map)
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

void UxRequestHandler::parseKVMap(KVMap&& map)
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
	case ControlType::Toggle: break;
	case ControlType::Label: break;
	default: ;
	}
}

void UxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
	if (req.getMethod().compare("POST") == 0)
	{
		std::string url = req.getURI();
		if (url.compare("/update") == 0)
		{
			if (!controllers->hasUpdate())
			{
				resp.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);
				resp.send();
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
	else
	{
		resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
		resp.setContentType("text/html");

		std::ostream& out = resp.send();
		std::ifstream file("cp\\index.html");
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
