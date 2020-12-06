#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QThread>
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
	
    ~MainWindow();
public slots:
	void setTemperature(const float temp);
	void setHumidity(const float humidity);
	void addLogLine(QString str);
	void toggleStatusConsole(bool visible);

private:
    Ui::MainWindow *ui;
	ServerWrapper *server;
	LoggingAccess *log;
};

#endif // MAINWINDOW_H
