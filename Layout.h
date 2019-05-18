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
private:
	LinkedList<Widget*> widgets = LinkedList<Widget*>();
	char* name;
	
 public:
	 virtual void OnGetInFocus(Renderer& renderer) {
		 DrawActiveIndicator(renderer);
	 };
	virtual void Down(Renderer& renderer) = 0;
	virtual void Up(Renderer& renderer) = 0;
	virtual void Ok(Renderer& renderer) = 0;
	virtual void DrawActiveIndicator(Renderer& renderer) {
		
	}
	virtual void Draw(Renderer& renderer) = 0;
	virtual void Back(Renderer& renderer) = 0;
	void Add(Widget* w) {
		widgets.add(w);
	}
	virtual LinkedList<Widget*>* GetChildren() override {
		return &widgets;
	}
	//virtual ~Layout()=0;
	
};



#endif

