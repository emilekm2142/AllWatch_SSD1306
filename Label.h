// Label.h

#ifndef _LABEL_h
#define _LABEL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Widget.h"

class Label :public Widget {
private:
	Widget* parent;
public:
	String text;
	
	Label(const char* a) {
		this->text = String(a);
	}
	virtual void Draw(Renderer& renderer) override {
		renderer.DrawString(x, y, (char*)text.c_str());
	}
	virtual int GetHeight() override{
		return 1;
	}
	virtual int GetWidth() override{
		return 1;
	}
};
#endif

