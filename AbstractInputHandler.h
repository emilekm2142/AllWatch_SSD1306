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
 protected:


 public:
	 AbstractInputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)());
	 virtual void OnLoop() = 0;
};



#endif

