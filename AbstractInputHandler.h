// InputHandler.h

#ifndef _ABSINPUTHANDLER_h
#define _ABSINPUTHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"
//Abstract, OnLoop has to be called in the main loop();
class AbstractInputHandler
{
protected:

	void(*OnOk)();
	void(*OnUp)();
	void(*OnDown)();
	void(*OnDataIncoming)();
	void(*OnBack)();
  Renderer* r;
 protected:


 public:
	 AbstractInputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(), void(*OnBack)(), Renderer* r) {
		 this->OnDown = OnDown;
		 this->OnOk = OnOk;
		 this->OnUp = OnUp;
    this->r=r;
		 this->OnBack = OnBack;
	 }

	 virtual void OnLoop() = 0;
};



#endif
