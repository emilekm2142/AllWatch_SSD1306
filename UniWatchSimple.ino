
/*
    Name:       UniWatchSimple.ino
    Created:	18-Mar-19 17:20:19
    Author:     DESKTOP-VQ942UG\Emile


	To port to another display, inherit from Renderer and write the implementations of virtual functions; then change the references in this file.
	To change the input mode, either modify existing inputHandler or write your own. It has to inherit from AbstractInputHandler.

	Requires LinkedList by IvanSaidel-https://github.com/ivanseidel/LinkedList

*/


#include <Arduino.h>
#include "SimpleMenuRecord.h"
#include "Column.h"
#include "Label.h"
#include "SSD1306Renderer.h"
#include "Widget.h"
#include "Layout.h"
#include "InputHandler.h"
#include "TimeKepper.h"
#include "Renderer.h"
#include "UserInterface.h"
#include "InputHandler.h"
#include "AbstractInputInterpreter.h"
#include "AbstractInputHandler.h"
#include "Column.h"
#include "Label.h"


void onOk() {
	UserInterface.Ok();
}
void onUp() {
	UserInterface.Up();
}
void onDown() {
	
	UserInterface.Down();
}

InputHandler inputHandler = InputHandler(
	&onOk,
	&onUp,
	&onDown,
	Serial);

Column menu;
auto r = SSD1306Renderer();
void setup() {
	Serial.begin(115200);
	
	r.init();

	//UserInterface.SetRenderer(r);

	/*auto l1 = new Label("mama!");
	auto l2 = new Label("mama2");
	auto l3 = new Label("mama3");
	auto l4 = new Label("mama3");
	l2->canBeActivated = true;
	l2->onActivate = [] {Serial.println("option L2"); };
	l1->canBeActivated = true;
	l1->onActivate = [] {};
	l3->canBeActivated = true;
	l3->onActivate = [] {Serial.print("asdasd"); };
	menu.Add(*l1);
	menu.Add(*l2);
	menu.Add(*l3);

	UserInterface.ShowLayout(menu);*/


}
//int freeRam() {
//	extern int __heap_start, *__brkval;
//	int v;
//	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
//}
void loop() {
	inputHandler.OnLoop();
	//Serial.println(freeRam());
}