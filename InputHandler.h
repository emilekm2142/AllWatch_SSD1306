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
#include "Renderer.h"
class InputHandler:public AbstractInputHandler
{
private:
	int debounceDelay = DEBOUNCE_TIME;
  long buttons[2][4]={
       {PIN_DOWN, 0, 0, 0},
    {PIN_UP, 0, 0, 0}
 
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
	InputHandler(void(*OnOk)(), void(*OnUp)(), void(*OnDown)(), void(*OnBack)(), Stream& Serial, Renderer* r) :AbstractInputHandler(OnOk, OnUp, OnDown, OnBack, r) {
		
#ifdef USE_TX_RX_AS_GPIO
		//https://arduino.stackexchange.com/questions/29938/how-to-i-make-the-tx-and-rx-pins-on-an-esp-8266-01-into-gpio-pins?rq=1
		//********** CHANGE PIN FUNCTION  TO GPIO **********
		//GPIO 1 (TX) swap the pin to a GPIO.
		
		pinMode(downPin, FUNCTION_3);
		//GPIO 3 (RX) swap the pin to a GPIO.
		pinMode(upPin, FUNCTION_3);
		//**************************************************
#endif
		#ifdef BUTTON_MODE_PULLUP
		pinMode(downPin, INPUT_PULLUP);
		pinMode(upPin, INPUT_PULLUP);
		#else
    pinMode(downPin, INPUT);
    pinMode(upPin, INPUT);
    #endif
	
		pinMode(0, INPUT_PULLUP);
		pinMode(2, INPUT_PULLUP);

		
		this->Serial2 = &Serial;

	}
  int ignoreFirstXImpulses=7;
	int ignored=0;
  void LongPress(int buttonID){
    switch(buttonID){
      case 1: this->OnOk(); break;
      case 0: this->OnBack(); break;
      }
  }
  void ShortPress(int buttonID){
  switch(buttonID){
      case 1: this->OnUp(); break;
      case 0: this->OnDown(); break;
      }
  }
	void OnLoop() override
	{
    if (ignored<ignoreFirstXImpulses){
      ignored++;
      return;
    }

    for (int j=0; j<2; j++){
      
      buttons[j][2] = buttons[j][1];
      buttons[j][1] = digitalRead(buttons[j][0]);
    
      #ifdef INVERT
      buttons[j][1] = !buttons[j][1];
      #endif
      //if (buttons[j][1]) Serial.printf("%d \n", j);
    }
    
    
      for (int i=0; i<2; i++){
     // Serial.printf("%d, %d \n", buttons[i][1],buttons[i][2]);
      if (buttons[i][1] && !buttons[i][2] &&!block){
         Serial.println("register new");
          buttons[i][3] = millis();
      }
    
         
      if (buttons[i][1] && !block){
        long timeDifference = abs(buttons[i][3] - millis());
        if (timeDifference>700){
            block = true;
            Serial.println("long press");
            LongPress(i);
           
          }
      }
      else if (!buttons[i][1] && !block && buttons[i][2]){
        block=true;
        Serial.println("short press");
        ShortPress(i);
       
      } 
      
    }
    	  if (!buttons[0][1] && !buttons[1][1]){
        block=false;  
      } 
   
    
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
