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

using namespace commproto;

void ServerWrapper::run()
{
	threadFunc();
}

void ServerWrapper::printTemp(variable::VariableBaseHandle& var)
{
	float temp = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit tempReady(temp);

	LOG_INFO("Temperature: %.2f C",temp);
}

void ServerWrapper::printHumidity(variable::VariableBaseHandle& var)
{
	float humidity = std::static_pointer_cast<variable::RealVariable>(var)->get();
	emit humidityReady(humidity);

	LOG_INFO("Humidity: %.2f%%", humidity);
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

void LoggingAccess::addLog(const char * line, const uint32_t size)
{
	QString linestr = QString::fromUtf8(line, size-1);
	emit addLogText(linestr);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->statusConsole->setReadOnly(true);
	
	log = new LoggingAccess();
	connect(log, &LoggingAccess::addLogText, this, &MainWindow::addLogLine, Qt::QueuedConnection);
	setLoggable(log);

	server = new ServerWrapper();
	connect(server, &ServerWrapper::tempReady, this, &MainWindow::setTemperature);
	connect(server, &ServerWrapper::humidityReady, this, &MainWindow::setHumidity);
	server->window = this;
	server->running = true;
	//log->moveToThread(server);
	server->start();
	
}

MainWindow::~MainWindow()
{
	if (server)
	{
		server->running = false;
		server->quit();
		server->wait();
	}
		
	if(server)
	{
		delete server;
		server = nullptr;
	}
	if(log)
	{
		delete log;
		log = nullptr;
	}
	delete ui;
}

void MainWindow::setTemperature(const float temp)
{
	QString tempStr;
	tempStr.sprintf("Temperature: %.2f *C",temp);
	ui->tempDisplay->setText(tempStr);
}

void MainWindow::setHumidity(const float humidity)
{
	QString tempStr;
	tempStr.sprintf("Humidity: %.2f%%", humidity);
	ui->humidityDisplay->setText(tempStr);
}

void MainWindow::addLogLine(QString str)
{
	
	ui->statusConsole->append(str);
}

