// 
// 
// 

#include "AbstractInputHandler.h"

AbstractInputHandler::AbstractInputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)()) {
	this->OnDown = OnDown;
	this->OnOk = OnOk;
	this->OnUp = OnUp;
}


