#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <SocketImpl.h>
#include <stdio.h>
#include <thread>
#include <commproto/common/Common.h>
#include <commproto/parser/ParserDelegatorFactory.h>
#include <commproto/parser/MessageBuilder.h>
#include <commproto/messages/MessageMapper.h>
#include "../../CommProtoLib/src/TypeMapperImpl.h"
#include <commproto/variable/ContextImpl.h>
#include <commproto/logger/Logging.h>
#include <commproto/variable/VariableMappingMessage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

using namespace commproto;

void ServerWrapper::run()
{
	threadFunc();
}

void ServerWrapper::printTemp(variable::VariableBaseHandle& var)
{
	temp = std::static_pointer_cast<variable::RealVariable>(var)->get();
	bool isIdeal = data.temp.isIdeal(temp);
	emit tempReady(temp, isIdeal);
	calculatePlantHealth();
	LOG_INFO("Temperature: %.2f C", temp);
}

void ServerWrapper::printHumidity(variable::VariableBaseHandle& var)
{
	humidity = std::static_pointer_cast<variable::RealVariable>(var)->get();
	bool isIdeal = data.humidity.isIdeal(humidity);
	emit humidityReady(humidity, isIdeal);
	calculatePlantHealth();
	LOG_INFO("Humidity: %.2f%%", humidity);
}

void ServerWrapper::printLight(variable::VariableBaseHandle& var)
{
	lightLuxes = std::static_pointer_cast<variable::RealVariable>(var)->get();
	bool isIdeal = data.sunlightHours.isIdeal(lightLuxes);
	bool isDay = isDayTime();
	bool turnOn = isDay && data.sunlightHours.getAdjustment(lightLuxes) < 0;;
	
	if (uv)
	{

		*uv = turnOn;
		LOG_INFO("Light difference = %3.2lf(%s)", data.sunlightHours.getAdjustment(lightLuxes), uv->get() ? "True" : "False");
	}
	emit lightReady(lightLuxes, isIdeal, turnOn, isDay);
	calculatePlantHealth();
	LOG_INFO("Light: %.2f luxes", lightLuxes);
}

void ServerWrapper::printSoilHumidity(variable::VariableBaseHandle& var)
{
	soilHumidity = std::static_pointer_cast<variable::RealVariable>(var)->get();
	bool isIdeal = data.soilHumidity.isIdeal(soilHumidity);
	bool turnOn = data.soilHumidity.getAdjustment(soilHumidity) < 0;
	if (irrigate)
	{

		*irrigate = turnOn;
		LOG_INFO("Irrigation difference = %3.2lf(%s)", data.soilHumidity.getAdjustment(soilHumidity), irrigate->get() ? "True" : "False");
	}
	emit soilHumidityReady(soilHumidity, isIdeal, turnOn);

	LOG_INFO("Soil Humidity: %.2f%%", soilHumidity);
	calculatePlantHealth();
}


class KeepAliveMsg : commproto::messages::MessageBase
{
public:
	KeepAliveMsg(const uint32_t id) : MessageBase{ id } {}
	uint32_t getSize() const override
	{
		return MessageBase::getSize();
	}
	static Message serialize(const uint32_t id)
	{
		KeepAliveMsg msg(id);
		parser::ByteStream stream;
		stream.writeHeader(msg);
		return stream.getStream();
	}
};


void ServerWrapper::threadFunc()
{
	uint32_t port = 4242;
	sockets::SocketHandle server = std::make_shared<sockets::SocketImpl>();
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
		sockets::SocketHandle client = server->acceptNext();
		if (!client)
		{
			printf("An error occurred while waiting for a connection.");
			continue;
		}
		client->setTimeout(1000);
		emit onConnection(true);
		Message msg;
		uint8_t sizeOfPtr = sizeof(void*);
		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		messages::TypeMapperObserverHandle observer = std::make_shared<messages::TypeMapperObserver>(client);
		
		messages::TypeMapperHandle mapper = std::make_shared<messages::TypeMapperImpl>(observer);
		uint32_t keepAliveId = mapper->registerType<KeepAliveMsg>();


		variable::ContextHandle ctx = std::make_shared<variable::ContextImpl>(client, mapper->registerType<variable::VariableMessage>(), mapper->registerType<variable::VariableMappingMessage>());

		variable::VariableCallback tempCb = std::bind(&ServerWrapper::printTemp, this, std::placeholders::_1);
		ctx->subscribe("TempC", tempCb);

		variable::VariableCallback humCb = std::bind(&ServerWrapper::printHumidity, this, std::placeholders::_1);
		ctx->subscribe("Humidity", humCb);

		variable::VariableCallback lightCb = std::bind(&ServerWrapper::printLight, this, std::placeholders::_1);
		ctx->subscribe("Light", lightCb);

		variable::VariableCallback soilHumCb = std::bind(&ServerWrapper::printSoilHumidity, this, std::placeholders::_1);
		ctx->subscribe("SoilHumidity", soilHumCb);


		irrigate = std::make_shared<variable::BoolVariable>(ctx, false);
		ctx->registerOutVariable(irrigate, "Irrigation");

		uv = std::make_shared<variable::BoolVariable>(ctx, false);
		ctx->registerOutVariable(uv, "UV");

		parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
		parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(client, delegator);
		uint32_t counter = 0;
		const uint32_t checkConnection = 5000;
		uint32_t sleepMsec = 1000;
		while (running)
		{
			
			builder->pollAndRead();
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMsec));
			counter += sleepMsec;
			if (counter == checkConnection) {
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

void ServerWrapper::setPlantData(PlantData data_)
{
	data = data_;
	LOG_INFO("Server received plant data for plant \"%s\"", data.name.c_str());
}

void ServerWrapper::getSunRiseSunSet()
{
}

bool ServerWrapper::isDayTime()
{
	return false;
}

const float RangeMeasure::tolerance = 0.5f;

bool RangeMeasure::isIdeal(const float value) const
{
	return value >= min - tolerance && value <= max + tolerance;
}

float RangeMeasure::getAdjustment(const float value) const
{
	if (value < min - tolerance)
	{
		return value - min;
	}
	if (value > max + tolerance)
	{
		return value - max;
	}
	return 0.0f;
}

float RangeMeasure::getPercentage(const float value) const
{

	if (value < criticalMin)
	{
		return 0.0f;
	}

	if (value < min - tolerance)
	{
		float distance = min - value;
		return (1.f - distance / (min - criticalMin)) * 100.0f;
	}

	if (value > criticalMax)
	{
		return 0.0f;
	}

	if (value > max + tolerance)
	{
		float distance = value - max;
		return (1.f - distance / (criticalMax - max)) * 100.0f;
	}

	return 100.0f;
}

void ServerWrapper::calculatePlantHealth()
{
	emit healthReady(data.temp.getPercentage(temp), data.humidity.getPercentage(humidity), data.soilHumidity.getPercentage(soilHumidity), data.sunlightHours.getPercentage(lightLuxes));
}

void LoggingAccess::addLog(const char * line, const uint32_t size)
{
	QString linestr = QString::fromUtf8(line, size - 1);
	emit addLogText(linestr);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	settings("app.ini", QSettings::IniFormat),
	tempIcon("icons/temperature.png"),
	humIcon("icons/humidity.png"),
	soilHumIcon("icons/soil.png"),
	lightIcon("icons/light.png"),
	plantIcon("icons/plant.png"),
	soilHumOnIcon("icons/soil_on.png"),
	lightOnIcon("icons/light_on.png"),
	plantYellowIcon("icons/plant_yellow.png"),
	plantRedIcon("icons/plant_red.png"),
	noLightIcon("icons/no_light.png"),
	connectedIcon("icons/connected.png"),
	notConnectIcon("icons/not_connected.png"),
	tempLabel(tempIcon),
	humLabel(humIcon),
	soilHumLabel(soilHumIcon),
	lightLabel(lightIcon),
	idealTempLabel(tempIcon),
	idealHumLabel(humIcon),
	idealSoilHumLabel(soilHumIcon),
	idealLightLabel(lightIcon),
	tempHealthLabel(tempIcon),
	humHealthLabel(humIcon),
	soilHumHealthLabel(soilHumIcon),
	lightHealthLabel(lightIcon),
	overallHealthLabel(plantIcon),
	connectionStatusLabel(notConnectIcon)

{
	ui->setupUi(this);
	setWindowIcon(plantIcon);
	resize(10, 10);

	ui->statusLayout->insertLayout(0, &connectionStatusLabel);
	ui->statusLayout->setAlignment(Qt::AlignLeft);

	ui->statusConsole->setReadOnly(true);

	ui->readingsLeft->setAlignment(Qt::AlignTop);

	setTemperature(0.f, false);
	setHumidity(0.f, false);
	setSoilHumidity(0.f, false, false);
	setLightExposure(0.f, false, false,false);
	setPlantHealth(0.f, 0.f, 0.f, 0.f);

	ui->readingsLeft->addLayout(&tempLabel);
	ui->readingsLeft->addLayout(&humLabel);
	ui->readingsLeft->addLayout(&soilHumLabel);
	ui->readingsLeft->addLayout(&lightLabel);

	clearUiValues();
	ui->readingsRight->setAlignment(Qt::AlignTop);
	ui->readingsRight->addLayout(&idealTempLabel);
	ui->readingsRight->addLayout(&idealHumLabel);
	ui->readingsRight->addLayout(&idealSoilHumLabel);
	ui->readingsRight->addLayout(&idealLightLabel);



	ui->plantHealthLayout->setAlignment(Qt::AlignTop);
	ui->plantHealthLayout->addLayout(&tempHealthLabel);
	ui->plantHealthLayout->addLayout(&humHealthLabel);
	ui->plantHealthLayout->addLayout(&soilHumHealthLabel);
	ui->plantHealthLayout->addLayout(&lightHealthLabel);
	ui->plantHealthLayout->addLayout(&overallHealthLabel);



	ui->readingsLayout->setAlignment(Qt::AlignLeft);
	ui->centralLayout->setAlignment(Qt::AlignTop);

	ui->measurementsLabel->setAlignment(Qt::AlignCenter);
	ui->idealValuesLabel->setAlignment(Qt::AlignCenter);

	logAccess = new LoggingAccess();
	connect(logAccess, &LoggingAccess::addLogText, this, &MainWindow::addLogLine, Qt::QueuedConnection);
	setLoggable(logAccess);

	server = new ServerWrapper();
	connect(server, &ServerWrapper::tempReady, this, &MainWindow::setTemperature);
	connect(server, &ServerWrapper::humidityReady, this, &MainWindow::setHumidity);
	connect(server, &ServerWrapper::lightReady, this, &MainWindow::setLightExposure);
	connect(server, &ServerWrapper::healthReady, this, &MainWindow::setPlantHealth);
	connect(server, &ServerWrapper::soilHumidityReady, this, &MainWindow::setSoilHumidity);
	connect(this, &MainWindow::plantDataReady, server, &ServerWrapper::setPlantData);
	connect(server, &ServerWrapper::onConnection, this, &MainWindow::onConnectionStatus);

	server->window = this;
	server->running = true;

	toggleStatusConsole(false);
	connect(ui->actionShow_console, &QAction::toggled, this, &MainWindow::toggleStatusConsole);
	connect(ui->actionLoad_configuration_file, &QAction::triggered, this, &MainWindow::onLoadFromJson);
	server->start();
	onLoadSettings();

}

void MainWindow::updateUiValues()
{
	idealTempLabel.text->setText(QString("Ideal ambiental temperature: ") + data.temp.toString().c_str());
	idealLightLabel.text->setText(QString("Ideal ambiental sunlight: ") + data.sunlightHours.toString().c_str());
	idealSoilHumLabel.text->setText(QString("Ideal soil humidity: ") + data.soilHumidity.toString().c_str());
	idealHumLabel.text->setText(QString("Ideal ambiental humidity: ") + data.humidity.toString().c_str());
	ui->plantName->setText(QString("Plant name: ") + data.name.c_str());
}

void MainWindow::clearUiValues()
{
	idealTempLabel.text->setText(QString("Ideal ambiental temperature: "));
	idealLightLabel.text->setText(QString("Ideal ambiental sunlight: "));
	idealSoilHumLabel.text->setText(QString("Ideal soil humidity: "));
	idealHumLabel.text->setText(QString("Ideal ambiental humidity: "));
	ui->plantName->setText(QString("Plant name: "));
}

void MainWindow::onLoadSettings()
{
	if (!settings.contains("loaded_plant_file"))
	{
		settings.setValue("loaded_plant_file", false);
	}
	else
	{
		if (settings.value("loaded_plant_file", false).toBool())
		{
			if (settings.contains("plant_file"))
			{
				loadFromJson(settings.value("plant_file", "").toString());
			}
			else
			{
				settings.setValue("loaded_plant_file", false);
			}
		}
	}

}

void MainWindow::resetValues()
{
	setTemperature(0.f, false);
	setHumidity(0.f, false);
	setSoilHumidity(0.f, false, false);
	setLightExposure(0.f, false, false, false);
	setPlantHealth(0.f, 0.f, 0.f, 0.f);
}

MainWindow::~MainWindow()
{
	if (server)
	{
		server->running = false;
		server->quit();
		server->wait();
	}

	if (server)
	{
		delete server;
		server = nullptr;
	}
	if (logAccess)
	{
		delete logAccess;
		logAccess = nullptr;
	}
	delete ui;
}


QString measureMentText(QString left, QString measurement, bool ideal)
{
	QString color = ideal ? "green" : "red";
	QString output;
	output.sprintf("%s: <font color=\"%s\">%s</font>", left.toUtf8().data(), color.toUtf8().data(), measurement.toUtf8().data());
	return output;
}

void MainWindow::setTemperature(const float temp, const bool good)
{
	QString tempStr;
	tempStr.sprintf("%2.2f *C", temp);
	tempLabel.text->setText(measureMentText("Ambiental temperature", tempStr, good));
}

void MainWindow::setHumidity(const float humidity, const bool good)
{
	QString tempStr;
	tempStr.sprintf("%3.2f%%", humidity);
	humLabel.text->setText(measureMentText("Ambiental humidity", tempStr, good));
}


void MainWindow::setLightExposure(const float light, const bool good, const bool uvOn, const bool isDayTime)
{
	QString tempStr;
	tempStr.sprintf("%4.2f luxes", light);
	lightLabel.text->setText(measureMentText("Ambiental light", tempStr, good));
	if (isDayTime) {
		if (uvOn)
		{
			lightLabel.setIcon(lightOnIcon);
		}
		else
		{
			lightLabel.setIcon(lightIcon);
		}
	}
	else
	{
		lightLabel.setIcon(noLightIcon);
	}
}

void MainWindow::setSoilHumidity(const float humidity, const bool good, const bool irrOn)
{
	QString tempStr;
	tempStr.sprintf("%3.2f%%", humidity);
	soilHumLabel.text->setText(measureMentText("Soil humidity", tempStr, good));
	if (irrOn)
	{
		soilHumLabel.setIcon(soilHumOnIcon);
	}
	else
	{
		soilHumLabel.setIcon(soilHumIcon);
	}
}

void MainWindow::setPlantHealth(const float temp_, const float hum_, const float soilHum_, const float light_)
{
	QString tempStr;
	tempStr.sprintf("%3.2f%%", temp_);
	QString humStr;
	humStr.sprintf("%3.2f%%", hum_);
	QString soilHumStr;
	soilHumStr.sprintf("%3.2f%%", soilHum_);
	QString lightStr;
	lightStr.sprintf("%3.2f%%", light_);

	float overall = (soilHum_ * 8.f + temp_ * 5.f + light_ * 5.f + hum_ * 2.f) / 20.f;

	QString overallStr;
	overallStr.sprintf("%3.2f%%", overall);

	tempHealthLabel.text->setText(measureMentText("Temperature", tempStr, temp_ > 70.0f));
	humHealthLabel.text->setText(measureMentText(" Humidity", humStr, hum_ > 70.0f));
	soilHumHealthLabel.text->setText(measureMentText(" Soil Humidity", soilHumStr, soilHum_ > 70.0f));
	lightHealthLabel.text->setText(measureMentText(" Light", lightStr, light_ > 70.0f));
	overallHealthLabel.text->setText(measureMentText(" Overall", overallStr, overall > 70.0f));
	if(overall > 70.f)
	{
		overallHealthLabel.setIcon(plantIcon);
	} 
	else if(overall > 30.f)
	{
		overallHealthLabel.setIcon(plantYellowIcon);
	} 
	else
	{
		overallHealthLabel.setIcon(plantRedIcon);
	}
}

void MainWindow::addLogLine(QString str)
{

	ui->statusConsole->append(str);
}

void MainWindow::toggleStatusConsole(bool visible)
{
	ui->statusConsole->setVisible(visible);
	ui->statusLabel->setVisible(visible);
	if (visible)
	{
		showMaximized();
	}
	else
	{
		showNormal();
		resize(10, 10);
	}
}


template <typename T>
bool setIfExistsArray(QJsonObject obj, const char *key, const int32_t index, const QJsonValue::Type expected, T& dest)
{
	QJsonValue value = obj[key];

	if (value.type() != QJsonValue::Array)
	{
		dest = T{};
		return false;
	}
	QJsonArray arr = value.toArray();

	if (arr.size() <= index)
	{
		dest = T{};
		return false;
	}

	if (arr[index].type() != expected)
	{
		dest = T{};
		return false;
	}

	switch (arr[index].type())
	{
	case QJsonValue::Double:
		dest = arr[index].toDouble(0);
		break;

	default:;
	}
	return true;
}


bool setRangeFromJson(QJsonObject obj, const char *key, RangeMeasure & dest)
{
	return setIfExistsArray(obj, key, 0, QJsonValue::Double, dest.min) &&
		setIfExistsArray(obj, key, 1, QJsonValue::Double, dest.max) &&
		setIfExistsArray(obj, key, 2, QJsonValue::Double, dest.criticalMin) &&
		setIfExistsArray(obj, key, 3, QJsonValue::Double, dest.criticalMax);
}

template <typename T>
bool setIfExists(QJsonObject obj, const char * key, const QJsonValue::Type expected, T& dest)
{
	QJsonValue value = obj[key];
	if (value.type() != expected)
	{
		return false;
	}
	switch (value.type()) {
	case QJsonValue::Double:
		dest = value.toDouble(0.0);
		break;
	case QJsonValue::String:
		dest = std::string(value.toString("NoName").toUtf8().data());
		break;
	default:;
	}
	return true;
}


void MainWindow::onLoadFromJson()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select Plant JSON File"),
		"",
		tr("JSON (*.json)"));
	loadFromJson(filePath);

}

void MainWindow::onConnectionStatus(const bool connected)
{
	if(connected)
	{
		connectionStatusLabel.setIcon(connectedIcon);
	}
	else
	{
		connectionStatusLabel.setIcon(notConnectIcon);
		resetValues();
	}
}

void MainWindow::loadFromJson(QString path)
{
	QFile file;
	file.setFileName(path);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString val = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject jObject = doc.object();
	bool res = setIfExists(jObject, "name", QJsonValue::String, data.name) &&
		setRangeFromJson(jObject, "ideal_temperature", data.temp) &&
		setRangeFromJson(jObject, "ideal_humidity", data.humidity) &&
		setRangeFromJson(jObject, "ideal_soil_humidity", data.soilHumidity) &&
		setRangeFromJson(jObject, "ideal_light_time", data.sunlightHours);
	if (res) {
		updateUiValues();
		settings.setValue("plant_file", path);
		settings.setValue("loaded_plant_file", true);
		emit plantDataReady(data);
		LOG_INFO("Loaded plant data file \"%s\"", path.toUtf8().data());
	}
	else
	{
		clearUiValues();
		settings.setValue("loaded_plant_file", false);
		LOG_ERROR("An error occurred while parsing plant data file \"%s\"", path.toUtf8().data());
	}
}

