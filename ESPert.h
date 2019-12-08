// ESPert.h

#ifndef _ESPERT_h
#define _ESPERT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Renderer.h"
#include "Buzzer.h"


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
	 Buzzer* buzzer;
	 ESPert(Renderer* r, Buzzer* buzzer) {
		 this->buzzer = buzzer;
		 this->oled = OledCompatibility(r);
	}
};


#endif

