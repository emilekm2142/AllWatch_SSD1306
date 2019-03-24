// 
// 
// 

#include "InputHandler.h"
InputHandler::InputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(), HardwareSerial& Serial) :AbstractInputHandler(OnOk, OnUp, OnDown) {
	
	this->Serial = &Serial;
}
 void InputHandler::OnLoop()
{
	 if (Serial->available() > 0) {
		 Serial->print((char)Serial->read());
	 }

}

