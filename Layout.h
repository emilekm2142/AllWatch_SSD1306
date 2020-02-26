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
protected:
	
 public:
	 std::function<void()> onBackCallback = nullptr;
	 std::function<void()> onDownCallback = nullptr;
	 std::function<void()> onUpCallback = nullptr;
	 std::function<void()> onOkCallback = nullptr;

	 virtual void GetInFocus(Renderer& renderer) {}
	 virtual void OnGetInFocus(Renderer& renderer) {
		 GetInFocus(renderer);
		 DrawActiveIndicator(renderer);
	 };
	 virtual void Down(Renderer& renderer) { if (onDownCallback != nullptr) onDownCallback(); }
	virtual void Up(Renderer& renderer) { if (onUpCallback != nullptr) onUpCallback(); }
	virtual void Ok(Renderer& renderer) { if (onOkCallback != nullptr) onOkCallback(); }
	virtual void DrawActiveIndicator(Renderer& renderer) {
		
	}
	virtual void Draw(Renderer& renderer) = 0;
	virtual void Back(Renderer& renderer) { if (onBackCallback != nullptr) onBackCallback(); };
	void Add(Widget* w) {
		widgets.add(w);
	}
	virtual LinkedList<Widget*>* GetChildren() override {
		return &widgets;
	}
	//virtual ~Layout()=0;
	
};



#endif

