#ifndef SDK_WRAPPER_H
#define SDK_WRAPPER_H

#include <commproto/variable/Variable.h>

class SDKWrapper
{
public:
	virtual ~SDKWrapper() = default;
	virtual void delayMs(const int msec) = 0;
	virtual void setLED(commproto::variable::VariableBaseHandle & var) = 0;
	virtual void setLed(const uint32_t pos, char red, char green, char blue) = 0;
	virtual void toggleIrrigation(const bool) = 0;
	virtual void toggleUVLight(const bool) = 0;
};


#endif