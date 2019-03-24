// SSD1306Renderer.h

#ifndef _SSD1306RENDERER_h
#define _SSD1306RENDERER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"

class SSD1306Renderer:public Renderer
{
private:
	//Adafruit_SSD1306* OLED;
	int currentColor = 1;
 protected:


 public:
	void init();
	virtual void DrawRectangle(int x, int y, int height, int width) override;
	virtual void DrawCircle(int x, int y, int radius) override;
	virtual void FillRectangle(int x, int y, int height, int width) override;
	virtual void DrawBitmap(int x, int y, char* &bitmap) override;
	virtual void DrawString(int x, int y, char* string) override;
	virtual int GetStringWidth(char* string) override;
};



#endif

