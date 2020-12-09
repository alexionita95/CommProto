#ifndef SERVERAPP_PLANT_DATA_H
#define SERVERAPP_PLANT_DATA_H
#include <QMetaType>
#include <string>

struct RangeMeasure
{
	static const float tolerance;
	float min, max;
	float criticalMin, criticalMax;
	std::string unitOfMeasure;

	bool isIdeal(const float value) const;

	float getAdjustment(const float value) const;

	float getPercentage(const float value) const;


	std::string toString() const;
};

struct PlantData
{
	std::string name;
	RangeMeasure temp;
	RangeMeasure humidity;
	RangeMeasure soilHumidity;
	RangeMeasure sunlightHours;
	PlantData();
};



Q_DECLARE_METATYPE(PlantData);

#endif //SERVERAPP_PLANT_DATA_H