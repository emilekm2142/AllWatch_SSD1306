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
	 int spacing = 4;
	 
	 
 public:
	 Column() {

	 }
	 Column(int spacing) {
		 this->spacing = spacing;
	 }
	

	 
	 virtual int CalculateHeight(Renderer& renderer) override {
		 int cursorY = y;
		 int cursorX = x;
		 for (int i = 0; i < GetChildren()->size(); i++) {
			 auto w = GetChildren()->get(i);
			 cursorY += spacing + w->GetHeight(renderer);
		 }
		 return cursorY;
	 }
	 virtual int CalculateWidth(Renderer& renderer) override {
		 int max = 0;
		 for (int i = 0; i < GetChildren()->size(); i++) {
			 auto w = GetChildren()->get(i);
			 if (w->GetWidth(renderer) > max) {
				 max = w->GetWidth(renderer);
			 }
		 }
		 return max;
	 }
	 virtual void Back(Renderer& r) override {

	 }
	 virtual void Down(Renderer& renderer) override {
		 int ls = selection;
		 int l = GetChildren()->size();
		 for (;;) {
			 ls++;
			 if (ls < 0) {
				 break;
			 }
			 if (ls >= l) {
				 break;
			 }
			 if (GetChildren()->get(ls)->canBeActivated) {
				 selection = ls;
				 GetChildren()->get(ls)->isHighlighted = true;
				 DrawWithState(renderer);
				 break;
			 }
		 }
	}
	 virtual void Up(Renderer& renderer) override {
		 int ls = selection;
		 int l = GetChildren()->size();
		 for (;;) {
			 ls--;
			 if (ls < 0) {
				 break;
			 }
			 if (ls >= l) {
				 break;
			 }
			 if (GetChildren()->get(ls)->canBeActivated) {
				 selection = ls;
				 GetChildren()->get(ls)->isHighlighted = true;
				 DrawWithState(renderer);
				 break;
			 }
		 }
	 }
	 virtual void Ok(Renderer& renderer) override {
		 auto elem = GetChildren()->get(selection);
		 if (elem->canBeActivated) elem->onActivate();
	 }
	 virtual ~Column() {
		 for (int i = 0; i <= GetChildren()->size(); i++) {
			 delete GetChildren()->get(i);
		 }
		 GetChildren()->clear();
	 }
	 virtual void Draw(Renderer& renderer) override {
		 
		// Serial.println("Object type: Column");
		 for (int i = 0; i < GetChildren()->size(); i++) {
			 auto w = GetChildren()->get(i);
			 w->DrawWithState(renderer);
		 }
		// renderer.Update();
		 

	 }
	 
	 virtual void CalculateLayout(Renderer& renderer) override {
		 CalculateSizesPostOrderTreeWalk(renderer);

		 int cursorY = y;
		 int cursorX = x;
		
		 for (int i = 0; i < GetChildren()->size(); i++) {
			
			 auto w = GetChildren()->get(i);
			 w->setX(cursorX);
			 w->setY(cursorY);

			 
			 cursorY += spacing + w->GetHeight(renderer);
		 }
		 ApplyGlobalCoordinatesPreorderTreeWalk(this, renderer);
		 Unmark();
		
	 }
};



#endif

