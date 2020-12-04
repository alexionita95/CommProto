#ifndef SDK_WRAPPER_H
#define SDK_WRAPPER_H

#include <commproto/variable/Variable.h>

class SDKWrapper
{
public:
	virtual ~SDKWrapper() = default;
	virtual void delay(const int msec) = 0;
	virtual void setLED(commproto::variable::VariableBaseHandle & var) = 0;
};


#endif