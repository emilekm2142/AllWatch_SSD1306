// InputHandler.h

#ifndef _INPUTHANDLER_h
#define _INPUTHANDLER_h
#include "AbstractInputHandler.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
//Implementation
class InputHandler:public AbstractInputHandler
{
private:
	Stream* Serial;
public:
	InputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(),Stream& Serial);
	void OnLoop() override;
};



#endif

