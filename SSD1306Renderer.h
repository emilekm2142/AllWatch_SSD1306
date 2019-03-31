// SSD1306Renderer.h

#ifndef _SSD1306RENDERER_h
#define _SSD1306RENDERER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#include <Arduino.h>
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"
#include <Wire.h>
#include <U8g2lib.h>
class SSD1306Renderer:public Renderer
{
private:
	U8G2_SSD1306_128X64_NONAME_1_HW_I2C  *d;
	int currentColor = 1;
 protected:


 public:
	 void init() {
		 
		 d = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 16, /* data=*/ 17);   // ESP32 Thing, HW I2C with pin remapping

		 d->begin();
		 d->setFont(u8g2_font_6x10_tf);
		 d->drawStr(5,5,"aaaa");
		 d->updateDisplay();
		
	 };
	virtual void DrawRectangle(int x, int y, int height, int width) override {
	};
	virtual void DrawCircle(int x, int y, int radius) override {};
	virtual void FillRectangle(int x, int y, int height, int width) override {};
	virtual void DrawBitmap(int x, int y, char* &bitmap) override {};
	virtual void DrawString(int x, int y, char* string) override {};
	virtual int GetStringWidth(char* string) override {};
};



#endif

