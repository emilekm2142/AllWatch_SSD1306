// Widget.h

#ifndef _WIDGET_h
#define _WIDGET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"
class Widget
{
protected:
	
	int x, y, width, height;
	int GlobalX, GlobalY;
public:
	bool canBeActivated = false;
	std::function<void()> onActivate;
	virtual void Draw(Renderer& renderer)=0;
	virtual int GetHeight();
	virtual int GetWidth();
	
	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	void setGlobalX(int x) {
		this->GlobalX = x;
	}
	void setGlobalY(int y) {
		this->GlobalY = y;
	}
};



#endif

