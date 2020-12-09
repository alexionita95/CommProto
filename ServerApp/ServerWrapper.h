#ifndef SERVER_APP_SERVER_WRAPPER_H
#define SERVER_APP_SERVER_WRAPPER_H
#include <QThread>
#include <QNetworkReply>
#include <commproto/variable/VariableBase.h>
#include "PlantData.h"
#include "commproto/variable/VariableHandler.h"

class MainWindow;

class ServerWrapper : public QThread
{
	Q_OBJECT
public:
	ServerWrapper();
	~ServerWrapper();
	MainWindow *window;
	bool running;

	void run() override;
	void printTemp(commproto::variable::VariableBaseHandle& var);
	void printHumidity(commproto::variable::VariableBaseHandle& var);
	void printLight(commproto::variable::VariableBaseHandle& var);
	void printSoilHumidity(commproto::variable::VariableBaseHandle& var);
	void threadFunc();
	void getSunRiseSunSet();
	bool isDayTime();

	public slots:
	void setPlantData(PlantData data_, const bool correct);
	void receivedSunriseSunsetResponse(QNetworkReply *reply);

signals:
	void tempReady(const float, const bool);
	void humidityReady(const float, const bool);
	void lightReady(const float, const bool, const bool, const bool);
	void soilHumidityReady(const float, const bool, const bool);
	void healthReady(const float, const float, const float, const float);
	void onConnection(const bool);
private:
	PlantData data;
	bool hasPlantData;
	float temp;
	float humidity;
	float soilHumidity;
	float lightLuxes;
	commproto::variable::BoolVariableHandle irrigate;
	commproto::variable::BoolVariableHandle uv;
	QTime sunrise;
	QTime sunset;
	QNetworkAccessManager  *manager;
	void calculatePlantHealth();
};


#endif //SERVER_APP_SERVER_WRAPPER_H