#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H
#include <map>
#include <Poco/Net/HTTPRequestHandler.h>
#include <commproto/control/UxControllers.h>
#include <Poco/Net/HTTPResponse.h>


enum class ControlType : uint8_t
{
	Button = 0,
	Slider,
	Toggle,
	Label
};

using KVMap = std::map<std::string, std::string>;

class UxRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	UxRequestHandler(const commproto::control::ux::UxControllersHandle & controllers) : controllers{ controllers }
	{

	}

	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;

private:
	void handleBase(const KVMap& map, std::string& connection, uint32_t& controlId) const;
	void handleButton(KVMap&& map);
	void handleToggle(KVMap&& map);
	void parseKVMap(KVMap&& map);
	commproto::control::ux::UxControllersHandle controllers;

};


#endif// HTTP_REQUEST_HANDLER_H