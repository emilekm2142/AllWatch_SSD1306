// CustomScreen.h

#ifndef _CUSTOMSCREEN_h
#define _CUSTOMSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
class CustomScreen:public Layout{
public:
	
	virtual int CalculateHeight(Renderer& renderer) override {
		return height;
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return width;
	}
	virtual void Up(Renderer& renderer) override {}
	virtual void Down(Renderer& renderer) override {}
	virtual void Ok(Renderer& renderer) override {}
	virtual void Back(Renderer& renderer) override {}
	
	virtual void DrawActiveIndicator(Renderer &renderer) override {
		//Serial.println("Drawing generic indicator");
		renderer.FillRectangle(0, renderer.GetScreenHeight() - 1, 1, renderer.GetScreenWidth());
	}

	virtual void CalculateLayout(Renderer& renderer) override {
		
	}
};



#endif

