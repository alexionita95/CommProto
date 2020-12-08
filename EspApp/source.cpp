#include <DeviceWrapper.h>
#include <commproto/logger/Logging.h>
#include <thread>


class Desktop : public SDKWrapper{
public:
	void delayMs(const int msec) override
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(msec));
	}
	void setLED(commproto::variable::VariableBaseHandle & var) override
	{
	
		bool led_state = false;
		switch (var->getType())
		{
		case variable::ValueType::bool8:
			led_state = std::static_pointer_cast<variable::BoolVariable>(var)->get();
			break;
		default:;
		}
		LOG_INFO("toggling led state to %s", led_state ? "on" : "off");
	}
	void setLed(const uint32_t pos, char red, char green, char blue) override
	{
	}
	void toggleIrrigation(const bool) override{
	}
	void toggleUVLight(const bool) override {
	}
};


Desktop desk;
DeviceWrapper wrapper(&desk,"127.0.0.1",4242);


void setup(){

}


int counter = 0;
int delay = 500;
int counterTarget = 2000;

void loop(){
	wrapper.loop();
	counter += delay;
	if(counter == counterTarget)
	{
		counter = 0;
		wrapper.setTemp(100.0f);
		wrapper.setHumidity(50.0f);
	}

	desk.delayMs(delay);
}

int main(int argc, const char * argv[])
{
    setup();
    while(true){
    loop();
    }
    return 0;
}