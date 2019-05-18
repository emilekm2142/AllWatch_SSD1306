// InputHandler.h

#ifndef _ABSINPUTHANDLER_h
#define _ABSINPUTHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Abstract, OnLoop has to be called in the main loop();
class AbstractInputHandler
{
protected:
	void(*OnOk)();
	void(*OnUp)();
	void(*OnDown)();
	void(*OnDataIncoming)();
	void(*OnBack)();
 protected:


 public:
	 AbstractInputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(), void(*OnBack)()) {
		 this->OnDown = OnDown;
		 this->OnOk = OnOk;
		 this->OnUp = OnUp;
		 this->OnBack = OnBack;
	 }

	 virtual void OnLoop() = 0;
};



#endif

