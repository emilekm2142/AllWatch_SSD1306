// SSD1306Renderer.h
//This class is responsible for both SSD1306 and SSH1106
#ifndef _SSD1306RENDERER_h
#define _SSD1306RENDERER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#include <Arduino.h>
#else
	#include "WProgram.h"
#endif

#include "Config.h"
#include "Renderer.h"
//#include <brzo_i2c.h>

#ifdef Renderer_SSD1306
	#include <SSD1306Wire.h> 
#endif 

#ifdef Renderer_SSH1106
	#include <SH1106Wire.h> 
#endif // SSH1106

class SSD1306Renderer:public Renderer
{
private:

	OLEDDisplay *d;
	
	int currentColor = 1;
 protected:


 public:
	 void init() {
		#ifdef Renderer_SSD1306
		 d = new SSD1306Wire(0x3c, SSD1306_PIN_SDA, SSD1306_PIN_SCL);
		#endif 
		#ifdef Renderer_SSH1106
			d = new SH1106Wire(0x3c, SSH1106_PIN_SDA, SSH1106_PIN_SCL);
		#endif // SSH1106
		 d->init();
		 d->flipScreenVertically();
		 d->setContrast(100);
		 d->setBrightness(100);
		
	
		
	 };
	virtual void DrawRectangle(int x, int y, int height, int width) override {
		d->drawRect(x, y, width, height);
	
	};
	virtual void DrawCircle(int x, int y, int radius) override {
		d->drawCircle(x, y, radius);
	};
	virtual void __SetColor(int color) override {
		d->setColor(color==1? OLEDDISPLAY_COLOR::BLACK: OLEDDISPLAY_COLOR::WHITE);
	}
	virtual void FillRectangle(int x, int y, int height, int width, bool backgroundColor=false) override {
		if (backgroundColor) d->setColor(OLEDDISPLAY_COLOR::BLACK);
		d->fillRect(x, y, width, height);
		if (backgroundColor) d->setColor(OLEDDISPLAY_COLOR::WHITE);
	};
	virtual int GetVerticalCenter() override {
		return GetScreenHeight() / 2;
	}
	virtual int GetHorizontalCenter() override {
		return GetScreenWidth() / 2;
	}
	virtual void DrawBitmap(int x, int y, int width, int height, const unsigned char* bitmap) override {
		d->drawXbm(x, y, width, height, bitmap);
	};
	virtual void DisableScreen() override {
		d->displayOff();
	}
	virtual void DrawString(int x, int y, const char* string, bool inversed = false) override {
		if (inversed) {
			d->setColor(OLEDDISPLAY_COLOR::INVERSE);
		}
		
		d->drawString(x, y, string);
		if (inversed) {
			d->setColor(OLEDDISPLAY_COLOR::WHITE);
		}
	};
	virtual int GetStringWidth(char* string) override {
		return d->getStringWidth(string);
	};
	virtual void Update() override {
		d->display();
	}
	virtual int GetLineHeight() override {
		return 8;
	}
	virtual void Clear() override {
		d->clear();
		
	}
	virtual int GetScreenWidth() override {
		return d->getWidth();
	}
	virtual int GetScreenHeight() override {
		return d->getHeight();
	}
	virtual void HardClear() override {
		d->clear();
		Update();
	}
	virtual void SetFont(uint8_t * f) override {
		d->setFont(f);
	}
	virtual void DrawXBM(int x, int y, int width, int height, unsigned char* bitmap) override {
		d->drawXbm(x, y, width, height, bitmap);
	}
	virtual void DrawAlignedString(int x, int y, const char* string, int width, int side) override {
		OLEDDISPLAY_TEXT_ALIGNMENT a;
		if (side == Left) {
			a = OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT;
		}
		if (side == Center) {
			a = OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_CENTER;
		}
		if (side == Right) {
			a = OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT;
		}
		d->setTextAlignment(a);
	
		d->drawStringMaxWidth(x, y, width, string);
		d->setTextAlignment(TEXT_ALIGN_LEFT);

	}
};



#endif

