#ifndef DEVICE_WRAPPER_H
#define DEVICE_WRAPPER_H

#include <commproto/variable/Context.h>
#include <commproto/parser/MessageBuilder.h>
#include <SocketImpl.h>
#include <SDKWrapper.h>


enum class DeviceState
{
	Searching,
	Initializing,
	Connected,
};


using namespace commproto;
class DeviceWrapper {
public:
	DeviceWrapper(SDKWrapper * device,const char * host, uint32_t port);
	void setup();
	DeviceState loop();
    void setTemp(float temp);
	void setHumidity(float humidity);
	void setLight(float lightPercentage);
	void setSoilHumidity(float humidityPercentage);

	void uvCallback(commproto::variable::VariableBaseHandle & var);
	void irrigationCallback(commproto::variable::VariableBaseHandle & var);
private:
	commproto::sockets::SocketHandle client;
	variable::ContextHandle ctx;
	parser::MessageBuilderHandle builder;
    variable::RealVariableHandle temp;
    variable::RealVariableHandle humidity;
    variable::RealVariableHandle soilHumidity;
    variable::RealVariableHandle lightPercentage;
	std::string host;
	uint32_t port;
	SDKWrapper* device;
	DeviceState state;
};
#endif 