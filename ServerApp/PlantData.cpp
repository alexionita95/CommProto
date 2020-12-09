#include "PlantData.h"



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

std::string RangeMeasure::toString() const
{
	char buf[255] = {0};
	sprintf(buf, "%.1f - %.1f %s", min, max, unitOfMeasure.c_str());
	return buf;
}

PlantData::PlantData()
{
	temp.unitOfMeasure = "*C";
	humidity.unitOfMeasure = "%";
	soilHumidity.unitOfMeasure = "%";
	sunlightHours.unitOfMeasure = "luxes";
}
