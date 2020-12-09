#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QThread>
#include <QSettings>
#include <commproto/variable/Variable.h>
#include <commproto/logger/Logging.h>
#include <QLabel>
#include <QHBoxLayout>


namespace Ui {
	class MainWindow;
}


struct RangeMeasure
{
	static const float tolerance;
	float min, max;
	float criticalMin, criticalMax;
	std::string unitOfMeasure;

	bool isIdeal(const float value) const;

	float getAdjustment(const float value) const;

	float getPercentage(const float value) const;


	std::string toString() const
	{
		char buf[255] = { 0 };
		sprintf(buf, "%.1f - %.1f %s", min, max, unitOfMeasure.c_str());
		return buf;

	}
};

struct PlantData
{
	std::string name;
	RangeMeasure temp;
	RangeMeasure humidity;
	RangeMeasure soilHumidity;
	RangeMeasure sunlightHours;
	PlantData()
	{
		temp.unitOfMeasure = "*C";
		humidity.unitOfMeasure = "%";
		soilHumidity.unitOfMeasure = "%";
		sunlightHours.unitOfMeasure = "luxes";
	}
};

Q_DECLARE_METATYPE(PlantData);

class MainWindow;

class ServerWrapper : public QThread
{
	Q_OBJECT
public:
	MainWindow *window;
	bool running;

	void run() override;
	void printTemp(commproto::variable::VariableBaseHandle& var);
	void printHumidity(commproto::variable::VariableBaseHandle& var);
	void printLight(commproto::variable::VariableBaseHandle& var);
	void printSoilHumidity(commproto::variable::VariableBaseHandle& var);
	void threadFunc();
	public slots:
	void setPlantData(PlantData data_);
signals:
	void tempReady(const float, const bool);
	void humidityReady(const float, const bool);
	void lightReady(const float, const bool,const bool);
	void soilHumidityReady(const float, const bool, const bool);
	void healthReady(const float, const float, const float, const float);
	void onConnection(const bool);
private:
	PlantData data;
	float temp;
	float humidity;
	float soilHumidity;
	float lightLuxes;
	commproto::variable::BoolVariableHandle irrigate;
	commproto::variable::BoolVariableHandle uv;


	void calculatePlantHealth();
};


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
	~MainWindow();
	public slots:
	void setTemperature(const float temp, const bool good);
	void setHumidity(const float humidity, const bool good);
	void setLightExposure(const float light, const bool good, const bool uvOn);
	void setSoilHumidity(const float humidity, const bool good, const bool irrOn);
	void setPlantHealth(const float temp_, const float hum_, const float soilHum_, const float light_);
	void addLogLine(QString str);
	void toggleStatusConsole(bool visible);
	void loadFromJson(QString path);
	void onLoadFromJson();
	void onConnectionStatus(const bool connected);
signals:
	void plantDataReady(PlantData data);
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
