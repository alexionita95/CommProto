#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSettings>
#include <commproto/variable/Variable.h>
#include <commproto/logger/Logging.h>
#include <QLabel>
#include <QHBoxLayout>
#include "ServerWrapper.h"

struct PlantData;

namespace Ui {
	class MainWindow;
}



class LabelWithIcon : public QHBoxLayout
{
public:
	QLabel *icon;
	QLabel *text;
	LabelWithIcon(const QIcon & icon_)
		: icon(new QLabel())
		, text(new QLabel())
	{
		icon->setPixmap(icon_.pixmap(30,30));
		addWidget(icon);
		addWidget(text);
		setAlignment(Qt::AlignLeft);
	}

	void setIcon(const QIcon & icon_)
	{
		icon->setPixmap(icon_.pixmap(30, 30));
	}

	~LabelWithIcon()
	{
		if (icon)
		{
			delete icon;
			icon = nullptr;
		}
		if (text)
		{
			delete text;
			text = nullptr;
		}
	}
};

class LoggingAccess : public QObject, public LoggableDestination
{
	Q_OBJECT
public:
	void addLog(const char * line, const uint32_t size) override;
signals:
	void addLogText(QString str);
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	void updateUiValues();
	void clearUiValues();
	void onLoadSettings();
	void onExistSaveSettings();
	void resetValues();
	~MainWindow();
	public slots:
	void setTemperature(const float temp, const bool good);
	void setHumidity(const float humidity, const bool good);
	void setLightExposure(const float light, const bool good, const bool uvOn, const bool isDayTime);
	void setSoilHumidity(const float humidity, const bool good, const bool irrOn);
	void setPlantHealth(const float temp_, const float hum_, const float soilHum_, const float light_);
	void addLogLine(QString str);
	void toggleStatusConsole(bool visible);
	void loadFromJson(QString path);
	void onLoadFromJson();
	void onConnectionStatus(const bool connected);
signals:
	void plantDataReady(PlantData data,const bool);
private:
	Ui::MainWindow *ui;
	ServerWrapper *server;
	LoggingAccess *logAccess;
	PlantData data;
	QSettings settings;
	QIcon tempIcon, humIcon, soilHumIcon, lightIcon, plantIcon,soilHumOnIcon,lightOnIcon,plantYellowIcon,plantRedIcon,noLightIcon;
	QIcon connectedIcon, notConnectIcon;
	LabelWithIcon tempLabel,
		humLabel,
		soilHumLabel,
		lightLabel,
		idealTempLabel,
		idealHumLabel,
		idealSoilHumLabel,
		idealLightLabel,
		tempHealthLabel,
		humHealthLabel,
		soilHumHealthLabel,
		lightHealthLabel,
		overallHealthLabel,
		connectionStatusLabel;
};

#endif // MAINWINDOW_H
