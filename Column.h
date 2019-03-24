// Column.h

#ifndef _COLUMN_h
#define _COLUMN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "SimpleMenuRecord.h"
class Column:public Layout
{
 protected:
	 int selection=0;
	 int spacing = 2;
	 LinkedList<Widget*> widgets = LinkedList<Widget*>();
	 virtual void ApplyGlobalCoordinates() override {

	 }
 public:
	 Column() {

	 }
	 Column(int spacing) {
		 this->spacing = spacing;
	 }
	

	 void Add(Widget& w) {
		 widgets.add(&w);
	 }
	 virtual void Down() override {
		 int ls = selection;
		 int l = widgets.size();
		 for (;;) {
			 ls++;
			 if (ls < 0) {
				 break;
			 }
			 if (ls >= l) {
				 break;
			 }
			 if (widgets.get(ls)->canBeActivated) {
				 selection = ls;
				 break;
			 }
		 }
	}
	 virtual void Up() override {
		 int ls = selection;
		 int l = widgets.size();
		 for (;;) {
			 ls--;
			 if (ls < 0) {
				 break;
			 }
			 if (ls >= l) {
				 break;
			 }
			 if (widgets.get(ls)->canBeActivated) {
				 selection = ls;
				 break;
			 }
		 }
	 }
	 virtual void Ok() override {
		 auto elem = widgets.get(selection);
		 if (elem->canBeActivated) elem->onActivate();
	 }
	 virtual ~Column() {
		 for (int i = 0; i <= widgets.size; i++) {
			 delete widgets.get(i);
		 }
		 widgets.clear();
	 }
	 virtual void Draw(Renderer& renderer) override {

	 }
	 virtual void CalculateLayout() override {
		 int cursorY = 0;
		 int cursorX = 0;
		 for (int i = 0; i <= widgets.size; i++) {
			 auto w = widgets.get(i);
			 w->setX(cursorX);
			 w->setY(cursorY);

			 
			 cursorY += spacing + w->GetHeight();
		 }
		 height = cursorY;
	 }
};



#endif

