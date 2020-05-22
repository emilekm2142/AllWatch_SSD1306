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
	int debounceDelay = DEBOUNCE_TIME;
  long buttons[2][4]={
    {PIN_UP, 0, 0, 0},
    {PIN_DOWN, 0, 0, 0}
    };
	int lastDownDebounceTime = 0;
	int lastUpDebounceTime = 0;

	int downPin = PIN_DOWN;
	int upPin = PIN_UP;

	Stream* Serial2;

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
		
	
		pinMode(0, INPUT_PULLUP);
		pinMode(2, INPUT_PULLUP);

		
		this->Serial2 = &Serial;

	}
  int ignoreFirstXImpulses=7;
	int ignored=0;

	void OnLoop() override
	{
    if (ignored<ignoreFirstXImpulses){
      ignored++;
      return;
    }

    for (int i=0; i<=1; i++){
      
      Serial.println(i);
      buttons[i][1] = digitalRead(buttons[i][0]);
      #ifdef INVERT
      buttons[i][1] = !buttons[i][1];
      #endif
    }
      for (int i=0; i<=1; i++){
      
      //if (i==1) Serial.println(buttons[i][1]); 
      if (buttons[i][1] && !buttons[i][2]){
         Serial.println("register new");
          buttons[i][3] = millis();
      }

      if (buttons[i][1]){
        long timeDifference = abs(buttons[i][3] - millis());
        Serial.println(timeDifference);
        if (timeDifference > longPressLimit){
          Serial.println("long press");
          int buttonID = i;

           switch(buttonID){
           case 0: this->OnOk();break;
            case 1:
              this->OnBack();
             break;
            }
            ignored=0;
             block=true;
             return;
        }
      }
     
      if (!buttons[i][1] && buttons[i][2] && !block){    
          Serial.println("short press");
          int buttonID = i;

           switch(buttonID){
            case 0: this->OnUp();ignored=0;break;
            case 1: this->OnDown();ignored=0;break;
            }
          }
          if (!buttons[0][1] && !buttons[1][1] && block){block=false;}
          buttons[i][2] = buttons[i][1];
          goto breaked;
        
      
      buttons[i][2] = buttons[i][1];
      
    }
    	
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
