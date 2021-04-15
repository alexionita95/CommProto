#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H
#include <map>
#include <Poco/Net/HTTPRequestHandler.h>
#include <commproto/control/ux/UxControllers.h>
#include <Poco/Net/HTTPResponse.h>


enum class ControlType : uint8_t
{
	Button = 0,
	Slider,
	Toggle,
	Label,
	Notification
};

using KVMap = std::map<std::string, std::string>;

class UxRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	UxRequestHandler(const commproto::control::ux::UxControllersHandle & controllers, const KVMap& mimetypes) : controllers{ controllers }, mimetypes {mimetypes}
	{
	}

	void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp) override;

private:
	void handleBase(const KVMap& map, std::string& connection, uint32_t& controlId) const;
	void handleButton(KVMap&& map) const;
	void handleToggle(KVMap&& map) const;
	void handleNotification(KVMap && map) const;
	void parseKVMap(KVMap&& map) const;
    
	commproto::control::ux::UxControllersHandle controllers;
    KVMap mimetypes;


};


#endif// HTTP_REQUEST_HANDLER_H