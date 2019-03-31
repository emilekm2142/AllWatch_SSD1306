// 
// 
// 

#include "InputHandler.h"
InputHandler::InputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(),Stream& Serial) :AbstractInputHandler(OnOk, OnUp, OnDown) {
	
	this->Serial = &Serial;
}
 void InputHandler::OnLoop()
{
	 if (Serial->available() > 0) {
		 auto d = (char)Serial->read();
		 if (d == 'd') {
			 this->OnDown();
		 }
		 Serial->print(d);
	 }

}

