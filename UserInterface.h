// UserInterface.h

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Widget.h"
#include "Layout.h"
#include "Renderer.h"
class UserInterfaceClass
{
private:
	Renderer* renderer;
	Layout* currentLayout;
	bool IsAnyLayoutDisplayed() {
		return currentLayout != nullptr;
	}
 protected:
	  
	 
	 
 public:
	 void SetRenderer(Renderer& rndr) {
		 this->renderer = &rndr;
	 }
	 void Down() {
		 if (IsAnyLayoutDisplayed()) {
			 currentLayout->Down();
		 }
		 
	 }
	 void Up() {
		 if (IsAnyLayoutDisplayed()) {
			 currentLayout->Up();
		 }
	 }
	 void Ok() {
		 if (IsAnyLayoutDisplayed()) {
			 currentLayout->Ok();
		 }
	 }
	 void ShowLayout(Layout& layout) {
		 currentLayout = &layout;
	}
	 void ClearMemory() {
		 delete currentLayout;
	 }
};

extern UserInterfaceClass UserInterface;

#endif

