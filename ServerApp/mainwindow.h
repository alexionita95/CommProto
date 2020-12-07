#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QThread>
#include <QSettings>
#include <commproto/variable/Variable.h>
#include <commproto/logger/Logging.h>


namespace Ui {
class MainWindow;
}

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

	void threadFunc();

	
signals:
	void tempReady(const float);
	void humidityReady(const float);
};


struct RangeMeasure
{
	static const float tolerance;
	float min, max;
	std::string unitOfMeasure;

	bool isIdeal(const float value) const;

	bool getAdjustment(const float value) const;
	std::string toString()
	{
		char buf[255]={0};
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
		temp.unitOfMeasure = "* C";
		humidity.unitOfMeasure = "%";
		soilHumidity.unitOfMeasure = "%";
		sunlightHours.unitOfMeasure = "hours";
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
	void setTemperature(const float temp);
	void setHumidity(const float humidity);
	void addLogLine(QString str);
	void toggleStatusConsole(bool visible);
	void loadFromJson(QString path);
	void onLoadFromJson();
private:
    Ui::MainWindow *ui;
	ServerWrapper *server;
	LoggingAccess *log;
	PlantData data;
	QSettings settings;
};

#endif // MAINWINDOW_H
