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

#include <QFile>
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
	float temp = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit tempReady(temp);

	LOG_INFO("Temperature: %.2f C", temp);
}

void ServerWrapper::printHumidity(variable::VariableBaseHandle& var)
{
	float humidity = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit humidityReady(humidity);

	LOG_INFO("Humidity: %.2f%%", humidity);
}

void ServerWrapper::printLight(variable::VariableBaseHandle& var)
{
	float temp = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit lightReady(temp);

	LOG_INFO("Light: %.2f luxes", temp);
}

void ServerWrapper::printSoilHumidity(variable::VariableBaseHandle& var)
{
	float humidity = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit soilHumidityReady(humidity);

	LOG_INFO("Soil Humidity: %.2f%%", humidity);
}

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
		Message msg;
		uint8_t sizeOfPtr = sizeof(void*);
		msg.emplace_back(sizeOfPtr);
		client->sendBytes(msg);

		messages::TypeMapperObserverHandle observer = std::make_shared<messages::TypeMapperObserver>(client);
		messages::TypeMapperHandle mapper = std::make_shared<messages::TypeMapperImpl>(observer);

		variable::ContextHandle ctx = std::make_shared<variable::ContextImpl>(client, mapper->registerType<variable::VariableMessage>(), mapper->registerType<variable::VariableMappingMessage>());

		variable::VariableCallback tempCb = std::bind(&ServerWrapper::printTemp, this, std::placeholders::_1);
		ctx->subscribe("TempC", tempCb);

		variable::VariableCallback humCb = std::bind(&ServerWrapper::printHumidity, this, std::placeholders::_1);
		ctx->subscribe("Humidity", humCb);

		variable::VariableCallback lightCb = std::bind(&ServerWrapper::printLight, this, std::placeholders::_1);
		ctx->subscribe("Light", lightCb);

		variable::VariableCallback soilHumCb = std::bind(&ServerWrapper::printSoilHumidity, this, std::placeholders::_1);
		ctx->subscribe("SoilHumidity", soilHumCb);

		parser::ParserDelegatorHandle delegator = parser::ParserDelegatorFactory::build(ctx);
		parser::MessageBuilderHandle builder = std::make_shared<parser::MessageBuilder>(client, delegator);
		while (running)
		{
			if (!client)
			{
				printf("An error occurred while waiting for a connection.");
				continue;
			}
			builder->pollAndRead();
			std::this_thread::sleep_for(std::chrono::seconds(2));
			if (!client->connected())
			{
				LOG_WARNING("***********************************************");
				LOG_WARNING("Disconnected from a connection, reinitializing.");
				client->close();
				break;
			}
		}
	}
}

const float RangeMeasure::tolerance = 0.5f;

bool RangeMeasure::isIdeal(const float value) const
{
	return value >= min - tolerance && value <= max + tolerance;
}

bool RangeMeasure::getAdjustment(const float value) const
{
	if (value < min - tolerance)
	{
		return min - value;
	}
	if (value > max + tolerance)
	{
		return value - max;
	}
	return 0.0f;
}

void LoggingAccess::addLog(const char * line, const uint32_t size)
{
	QString linestr = QString::fromUtf8(line, size - 1);
	emit addLogText(linestr);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	settings("app.ini",QSettings::IniFormat)
{
	ui->setupUi(this);
	ui->statusConsole->setReadOnly(true);
	ui->readingsLeft->setAlignment(Qt::AlignLeft);
	ui->readingsRight->setAlignment(Qt::AlignLeft);
	ui->readingsLayout->setAlignment(Qt::AlignTop);


	logAccess = new LoggingAccess();
	connect(logAccess, &LoggingAccess::addLogText, this, &MainWindow::addLogLine, Qt::QueuedConnection);
	setLoggable(logAccess);

	server = new ServerWrapper();
	connect(server, &ServerWrapper::tempReady, this, &MainWindow::setTemperature);
	connect(server, &ServerWrapper::humidityReady, this, &MainWindow::setHumidity);
	connect(server, &ServerWrapper::lightReady, this, &MainWindow::setLightExposure);
	connect(server, &ServerWrapper::soilHumidityReady, this, &MainWindow::setSoilHumidity);
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
	ui->idealTemperature->setText(QString("Ideal ambiental temperature: ") + data.temp.toString().c_str());
	ui->idealLight->setText(QString("Ideal ambiental sunlight: ") + data.sunlightHours.toString().c_str());
	ui->idealSoilHumidity->setText(QString("Ideal soil humidity: ") + data.soilHumidity.toString().c_str());
	ui->idealhumidityDisplay->setText(QString("Ideal ambiental humidity: ") + data.humidity.toString().c_str());
	ui->plantName->setText(QString("Name: ") + data.name.c_str());
}

void MainWindow::clearUiValues()
{
	ui->idealTemperature->setText(QString("Ideal ambiental temperature: "));
	ui->idealLight->setText(QString("Ideal ambiental sunlight: "));
	ui->idealSoilHumidity->setText(QString("Ideal soil humidity: "));
	ui->idealhumidityDisplay->setText(QString("Ideal ambiental humidity: "));
	ui->plantName->setText(QString("Name: "));
}

void MainWindow::onLoadSettings()
{
	if(!settings.contains("loaded_plant_file"))
	{
		settings.setValue("loaded_plant_file", false);
	}
	else
	{
		if(settings.value("loaded_plant_file",false).toBool())
		{
			if(settings.contains("plant_file"))
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

void MainWindow::setTemperature(const float temp)
{
	QString tempStr;
	tempStr.sprintf("Temperature: %.2f *C", temp);
	ui->tempDisplay->setText(tempStr);
}

void MainWindow::setHumidity(const float humidity)
{
	QString tempStr;
	tempStr.sprintf("Humidity: %.2f%%", humidity);
	ui->humidityDisplay->setText(tempStr);
}

void MainWindow::setLightExposure(const float light)
{
	QString tempStr;
	tempStr.sprintf("Ambiental light: %.2f luxes", light);
	ui->lightDisplay->setText(tempStr);
}

void MainWindow::setSoilHumidity(const float humidity)
{
	QString tempStr;
	tempStr.sprintf("Soil humidty: %.2f%%", humidity);
	ui->soilHumidityDisplay->setText(tempStr);
}

void MainWindow::addLogLine(QString str)
{

	ui->statusConsole->append(str);
}

void MainWindow::toggleStatusConsole(bool visible)
{
	ui->statusConsole->setVisible(visible);
	ui->statusLabel->setVisible(visible);
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
		setIfExistsArray(obj, key, 1, QJsonValue::Double, dest.max);
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
		LOG_INFO("Loaded plant data file \"%s\"", path.toUtf8().data());
	}
	else
	{
		clearUiValues();
		settings.setValue("loaded_plant_file", false);
		LOG_ERROR("An error occurred while parsing plant data file \"%s\"", path.toUtf8().data());
	}
}

