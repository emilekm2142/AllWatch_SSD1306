// Renderer.h

#ifndef _RENDERER_h
#define _RENDERER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Renderer
{
 public:
	 virtual int GetVerticalCenter() = 0;
	 virtual int GetHorizontalCenter() = 0;
	 int Left = 0, Center = 1, Right = 2;
	 virtual void init() = 0;
	virtual void DrawRectangle(int x, int y, int height, int width)=0;
	virtual void DrawCircle(int x, int y, int radius)=0;
	virtual void FillRectangle(int x, int y, int height, int width, bool backgroundColor = false)=0;
	virtual void DrawBitmap(int x, int y, int width, int height, const unsigned char* bitmap)=0;
	virtual void DrawXBM(int x, int y, int width, int height, unsigned char* xbm)=0;
	virtual void DrawString(int x, int y, const char* string, bool inversed=false)=0;
	virtual int GetStringWidth(char* string)=0;
	virtual void Update() = 0;
	virtual int GetLineHeight() = 0;
	virtual void DisableScreen() = 0;
	virtual void Clear() = 0;
	virtual int GetScreenWidth() = 0;
	virtual int GetScreenHeight() = 0;
	virtual void HardClear() = 0;
	virtual void SetFont(uint8_t * f) = 0;
	virtual void DrawAlignedString(int x, int y, const char* string,int width, int side) = 0;
	virtual void __SetColor(int color) = 0;
};



#endif

