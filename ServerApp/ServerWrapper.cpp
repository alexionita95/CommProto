#include "ServerWrapper.h"

#include <commproto/common/Common.h>
#include <commproto/messages/MessageBase.h>
#include "commproto/parser/ByteStream.h"
#include "commproto/variable/Variable.h"
#include "SocketImpl.h"
#include "../CommProtoLib/src/TypeMapperObserver.h"
#include "../CommProtoLib/src/TypeMapperImpl.h"
#include "commproto/variable/ContextImpl.h"
#include "commproto/variable/VariableMappingMessage.h"
#include "commproto/parser/ParserDelegator.h"
#include "commproto/parser/MessageBuilder.h"
#include "commproto/parser/ParserDelegatorFactory.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonObject>

class KeepAliveMsg : commproto::messages::MessageBase
{
public:
	KeepAliveMsg(const uint32_t id) : MessageBase{ id } {}
	uint32_t getSize() const override
	{
		return MessageBase::getSize();
	}
	static commproto::Message serialize(const uint32_t id)
	{
		KeepAliveMsg msg(id);
		commproto::parser::ByteStream stream;
		stream.writeHeader(msg);
		return stream.getStream();
	}
};




ServerWrapper::ServerWrapper() : hasPlantData(false), manager(new QNetworkAccessManager)
{
	connect(manager, &QNetworkAccessManager::finished, this, &ServerWrapper::receivedSunriseSunsetResponse);
	getSunRiseSunSet();
}

ServerWrapper::~ServerWrapper()
{
	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
}

void ServerWrapper::run()
{
	threadFunc();
}

void ServerWrapper::printTemp(commproto::variable::VariableBaseHandle& var)
{
	temp = std::static_pointer_cast<commproto::variable::RealVariable>(var)->get();
	if (hasPlantData) {
		bool isIdeal = data.temp.isIdeal(temp);
		emit tempReady(temp, isIdeal);
		calculatePlantHealth();
	}
	else
	{
		emit tempReady(temp, false);
	}
	LOG_INFO("Temperature: %.2f C", temp);
}

void ServerWrapper::printHumidity(commproto::variable::VariableBaseHandle& var)
{
	humidity = std::static_pointer_cast<commproto::variable::RealVariable>(var)->get();
	if (hasPlantData) {
		bool isIdeal = data.humidity.isIdeal(humidity);
		emit humidityReady(humidity, isIdeal);
		calculatePlantHealth();

	}
	else
	{
		emit humidityReady(humidity, false);
	}
	LOG_INFO("Humidity: %.2f%%", humidity);
}

void ServerWrapper::printLight(commproto::variable::VariableBaseHandle& var)
{
	lightLuxes = std::static_pointer_cast<commproto::variable::RealVariable>(var)->get();
	bool isDay = isDayTime();
	if (hasPlantData) {
		bool isIdeal = data.sunlightHours.isIdeal(lightLuxes);
		bool turnOn = isDay && data.sunlightHours.getAdjustment(lightLuxes) < 0;;

		if (uv)
		{

			*uv = turnOn;
			LOG_INFO("Light difference = %3.2lf(%s)", data.sunlightHours.getAdjustment(lightLuxes), uv->get() ? "True" : "False");
		}
		emit lightReady(lightLuxes, isIdeal, turnOn, isDay);
		calculatePlantHealth();
	}
	else
	{
		emit lightReady(lightLuxes, false, false, isDay);
	}
	LOG_INFO("Light: %.2f luxes", lightLuxes);
}

void ServerWrapper::printSoilHumidity(commproto::variable::VariableBaseHandle& var)
{
	soilHumidity = std::static_pointer_cast<commproto::variable::RealVariable>(var)->get();
	if (hasPlantData) {
		bool isIdeal = data.soilHumidity.isIdeal(soilHumidity);
		bool turnOn = data.soilHumidity.getAdjustment(soilHumidity) < 0;
		if (irrigate)
		{

			*irrigate = turnOn;
			LOG_INFO("Irrigation difference = %3.2lf(%s)", data.soilHumidity.getAdjustment(soilHumidity), irrigate->get() ? "True" : "False");
		}
		emit soilHumidityReady(soilHumidity, isIdeal, turnOn);
		calculatePlantHealth();
	}
	else
	{
		emit soilHumidityReady(soilHumidity, false, false);
	}
	LOG_INFO("Soil Humidity: %.2f%%", soilHumidity);

}



void ServerWrapper::threadFunc()
{
	uint32_t port = 4242;
	commproto::sockets::SocketHandle server = std::make_shared<commproto::sockets::SocketImpl>();
	LOG_INFO("Attempting to bind a socket to port %d...", port);
	bool init = server->initServer(port);
	if (!init)
	{
		LOG_ERROR("An error occurred while attempting to bind to port %d.", port);
		return;
	}
	LOG_INFO("Succesfully bound to port %d. Awaiting next connection...", port);

	bool toggle = true;
	while (running)
	{
		commproto::sockets::SocketHandle client = server->acceptNext();
		if (!client)
		{
			printf("An error occurred while waiting for a connection.");
			continue;
		}
		client->setTimeout(1000);
		emit onConnection(true);
		commproto::Message msg;
		uint8_t sizeOfPtr = sizeof(void*);
		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		commproto::messages::TypeMapperObserverHandle observer = std::make_shared<commproto::messages::TypeMapperObserver>(client);

		commproto::messages::TypeMapperHandle mapper = std::make_shared<commproto::messages::TypeMapperImpl>(observer);
		uint32_t keepAliveId = mapper->registerType<KeepAliveMsg>();


		commproto::variable::ContextHandle ctx = std::make_shared<commproto::variable::ContextImpl>(client, mapper->registerType<commproto::variable::VariableMessage>(), mapper->registerType<commproto::variable::VariableMappingMessage>());

		commproto::variable::VariableCallback tempCb = std::bind(&ServerWrapper::printTemp, this, std::placeholders::_1);
		ctx->subscribe("TempC", tempCb);

		commproto::variable::VariableCallback humCb = std::bind(&ServerWrapper::printHumidity, this, std::placeholders::_1);
		ctx->subscribe("Humidity", humCb);

		commproto::variable::VariableCallback lightCb = std::bind(&ServerWrapper::printLight, this, std::placeholders::_1);
		ctx->subscribe("Light", lightCb);

		commproto::variable::VariableCallback soilHumCb = std::bind(&ServerWrapper::printSoilHumidity, this, std::placeholders::_1);
		ctx->subscribe("SoilHumidity", soilHumCb);


		irrigate = std::make_shared<commproto::variable::BoolVariable>(ctx, false);
		ctx->registerOutVariable(irrigate, "Irrigation");

		uv = std::make_shared<commproto::variable::BoolVariable>(ctx, false);
		ctx->registerOutVariable(uv, "UV");

		commproto::parser::ParserDelegatorHandle delegator = commproto::parser::ParserDelegatorFactory::build(ctx);
		commproto::parser::MessageBuilderHandle builder = std::make_shared<commproto::parser::MessageBuilder>(client, delegator);
		uint32_t counter = 0;
		const uint32_t checkConnection = 5000;
		uint32_t sleepMsec = 1000;
		while (running)
		{

			builder->pollAndRead();
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMsec));
			counter += sleepMsec;
			if (counter >= checkConnection) {
				counter = 0;
				client->sendBytes(KeepAliveMsg::serialize(keepAliveId));
			}
			if (!client->connected())
			{
				LOG_WARNING("***********************************************");
				LOG_WARNING("Disconnected from a connection, reinitializing.");
				emit onConnection(false);
				client->close();
				break;
			}
		}
	}
}

void ServerWrapper::setPlantData(PlantData data_, const bool correct)
{
	if (!correct)
	{
		hasPlantData = false;
		return;
	}
	data = data_;
	hasPlantData = true;
	LOG_INFO("Server received plant data for plant \"%s\"", data.name.c_str());
}

void ServerWrapper::receivedSunriseSunsetResponse(QNetworkReply* reply)
{

	if (reply->error()) {
		return;
	}

	QString answer = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(answer.toUtf8());
	QJsonObject obj = doc.object();

	if (!obj.contains("results"))
	{
		return;
	}
	QJsonObject results = obj.find("results").value().toObject();

	if (!results.contains("sunrise") || !results.contains("sunset"))
	{
		return;
	}

	QString sunriseStr = results.find("sunrise").value().toString();
	QString sunsetStr = results.find("sunset").value().toString();

	QDateTime local(QDateTime::currentDateTime());
	QDateTime UTC(local.toUTC());
	QDateTime dt(UTC.date(), UTC.time(), Qt::LocalTime);
	int64_t diff = dt.secsTo(local);

	QTime responseSunrise = QTime::fromString(sunriseStr, "h:mm:ss A");
	QTime responseSunset = QTime::fromString(sunsetStr, "h:mm:ss A");
	sunrise = responseSunrise.addSecs(diff);
	sunset = responseSunset.addSecs(diff);
}

void ServerWrapper::getSunRiseSunSet()
{
	static const float lat = 45.64861f;
	static const float lng = 25.60613f;

	QString req;
	req.sprintf("%slat=%lf&lng=%lf&formatted=1", "https://api.sunrise-sunset.org/json?", lat, lng);
	QNetworkRequest request(req);
	manager->get(request);

}

bool ServerWrapper::isDayTime()
{
	QTime now = QTime::currentTime();
	return sunrise < now && now < sunset;
}


void ServerWrapper::calculatePlantHealth()
{
	float lightHealth = isDayTime() ? data.sunlightHours.getPercentage(lightLuxes) : 100.0f;
	emit healthReady(data.temp.getPercentage(temp), data.humidity.getPercentage(humidity), data.soilHumidity.getPercentage(soilHumidity), lightHealth);
}