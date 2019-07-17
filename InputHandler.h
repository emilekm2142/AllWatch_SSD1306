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
#include "Config.h"
class InputHandler:public AbstractInputHandler
{
private:

	int downPin = PIN_DOWN;
	int upPin = PIN_UP;

	Stream* Serial2;
	int downPin = PIN_DOWN;
	int upPin = PIN_UP;
	long longPressLimit = 500;

	long downPressedStartTime = 0;
	long upPressedStartTime = 0;

	bool downPressed = false;
	bool upPressed = false;

	bool wasDownPressed = false;
	bool wasUpPressed = false;

	bool block = false;

	bool isLongUpPress() {
		return longPressLimit < abs(millis() - upPressedStartTime);
	}
	bool isLongDownPress() {
		return longPressLimit < abs(millis() - downPressedStartTime);
	}

public:
	InputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(), void(*OnBack)(), Stream& Serial) :AbstractInputHandler(OnOk, OnUp, OnDown, OnBack) {
		
#ifdef USE_TX_RX_AS_GPIO
		//https://arduino.stackexchange.com/questions/29938/how-to-i-make-the-tx-and-rx-pins-on-an-esp-8266-01-into-gpio-pins?rq=1
		//********** CHANGE PIN FUNCTION  TO GPIO **********
		//GPIO 1 (TX) swap the pin to a GPIO.
		
		pinMode(downPin, FUNCTION_3);
		//GPIO 3 (RX) swap the pin to a GPIO.
		pinMode(upPin, FUNCTION_3);
		//**************************************************
#endif

		pinMode(downPin, INPUT_PULLUP);
		pinMode(upPin, INPUT_PULLUP);
		this->Serial2 = &Serial;

	}
	void OnLoop() override
	{
		downPressed = digitalRead(downPin) ? false : true;
		upPressed = digitalRead(upPin) ? false : true;
#ifdef INVERSE
		downPressed = !downPressed;
		upPressed = !upPressed;
#endif // DEBUG
		if (block && !downPressed && !upPressed) {
			block = false;
			wasDownPressed = false;
			downPressed = false;
			wasUpPressed = false;
			upPressed = false;
			goto breaked;
		}
		if (!block) {
			
			if (downPressed && !wasDownPressed) { downPressedStartTime = millis(); }
			if (upPressed && !wasUpPressed) { upPressedStartTime = millis(); }



			if (downPressed && wasDownPressed && isLongDownPress()) {
				OnOk();
				wasDownPressed = false;
				downPressed = false;
				block = true;
				goto breaked;
			}
			if (upPressed && wasUpPressed && isLongUpPress()) {
				OnBack();
				wasUpPressed = false;
				upPressed = false;
				block = true;
				goto breaked;
			}
			if (!downPressed && wasDownPressed) {
				OnDown();
			}
			if (!upPressed && wasUpPressed) {
				OnUp();
			}
		}
		
		wasDownPressed = downPressed;
		wasUpPressed = upPressed;
		breaked:
		if (Serial.available() > 0) {
			auto d = (char)Serial.read();
			if (d == 'd') {
				this->OnDown();
			}
			if (d == 'u') {
				this->OnUp();
			}
			if (d == 'o') {
				this->OnOk();
			}
			if (d == 'b') {
				this->OnBack();
			}
			// Serial->print(d);
		}

	}
};



#endif

