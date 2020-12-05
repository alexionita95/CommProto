#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <commproto/variable/Variable.h>

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
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
	void setTemperature(const float temp);
	void setHumidity(const float humidity);
private:
    Ui::MainWindow *ui;
	ServerWrapper * server;
};

#endif // MAINWINDOW_H
