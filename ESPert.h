// ESPert.h

#ifndef _ESPERT_h
#define _ESPERT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"



class OledCompatibility {
public:
	Renderer* r;
	OledCompatibility(Renderer* r) {
		this->r = r;
	}
	OledCompatibility(){}
	void setColor(int color) {
		r->__SetColor(color);
	}
	void drawBitmap(int x, int y, int width, int height, const unsigned char* bitmap, bool something) {
		r->DrawBitmap(x, y, width, height, (const unsigned char*) bitmap);
	}
	void clear(boolean something) {
		r->Clear();
	}
	void update() {
		r->Update();
	}
};

class ESPert
{
 protected:

	
 public:
	 OledCompatibility oled;
	 ESPert(Renderer* r) {
		 this->oled = OledCompatibility(r);
	}
};


#endif

