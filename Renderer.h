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
	 virtual void init() = 0;
	virtual void DrawRectangle(int x, int y, int height, int width)=0;
	virtual void DrawCircle(int x, int y, int radius)=0;
	virtual void FillRectangle(int x, int y, int height, int width)=0;
	virtual void DrawBitmap(int x, int y, char* &bitmap)=0;
	virtual void DrawString(int x, int y, char* string)=0;
	virtual int GetStringWidth(char* string)=0;

};



#endif

