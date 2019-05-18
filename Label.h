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
	virtual void DrawHighlighted(Renderer& renderer) override {
		//Serial.println("Object type: Label (h)");
		
		renderer.DrawString(GlobalX+9, GlobalY, (char*)text.c_str(),true);
	}
	virtual void Draw(Renderer& renderer) override {
		//Serial.println("Object type: Label");
		renderer.DrawString(GlobalX, GlobalY, (char*)text.c_str(),true);
	}
	virtual int CalculateHeight(Renderer& renderer) override{
		return renderer.GetLineHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override{
		return renderer.GetStringWidth((char*)text.c_str());
	}

};
#endif

