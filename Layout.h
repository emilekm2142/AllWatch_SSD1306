// Layout.h

#ifndef _LAYOUT_h
#define _LAYOUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Renderer.h"
#include "Widget.h"
class Layout :public Widget
{
 protected:
	 virtual void ApplyGlobalCoordinates() = 0;

 public:
	virtual void CalculateLayout() = 0;
	virtual void Down() = 0;
	virtual void Up() = 0;
	virtual void Ok() = 0;
	virtual void Draw(Renderer& renderer) = 0;
	//virtual ~Layout()=0;
	
};



#endif

